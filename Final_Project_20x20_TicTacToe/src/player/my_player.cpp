#include "my_player.hpp"
#include <climits>

namespace ttt::my_player {

void MyPlayer::set_sign(Sign sign) { m_sign = sign; }
const char *MyPlayer::get_name() const { return m_name; }

// Direction vectors: horizontal, vertical, and two diagonals
static const int DX[4] = {1, 0, 1,  1};
static const int DY[4] = {0, 1, 1, -1};

// Scoring weights - higher values for more dangerous patterns
static const int WIN      = 10000000;
static const int MY_OPEN4 =   500000;  // 4 in a row, both ends open
static const int MY_HALF4 =   100000;  // 4 in a row, one end blocked
static const int MY_OPEN3 =    10000;
static const int MY_HALF3 =     1000;
static const int MY_OPEN2 =      200;
static const int MY_HALF2 =       20;
static const int MY_OPEN1 =        5;
static const int OPP_OPEN4 =  475000;  // Opponent scores (95% of mine)
static const int OPP_HALF4 =   95000;
static const int OPP_OPEN3 =    9500;
static const int OPP_HALF3 =     950;
static const int OPP_OPEN2 =     190;
static const int OPP_HALF2 =      19;
static const int OPP_OPEN1 =       4;

// Board cell states
static const int CV_NONE     = 0;
static const int CV_MY       = 1;
static const int CV_OPP      = 2;
static const int CV_OBSTACLE = 3;

static inline bool in_bounds(int x, int y, int cols, int rows) {
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

// Custom board for minimax (allows undo operations)
struct Board {
    int cols, rows, win_len, size;
    int *cells;
    void alloc(int c, int r, int w) {
        cols = c; rows = r; win_len = w; size = c * r;
        cells = new int[size];
        for (int i = 0; i < size; i++) cells[i] = CV_NONE;
    }
    void free_mem() { delete[] cells; cells = nullptr; }
    int  get(int x, int y) const  { return cells[y * cols + x]; }
    void set(int x, int y, int v) { cells[y * cols + x] = v;   }
};

// Check if placing piece creates a winning line
static bool check_win(const Board &b, int px, int py, int v) {
    for (int d = 0; d < 4; d++) {
        int cnt = 1;
        for (int s = -1; s <= 1; s += 2) {
            int x = px + DX[d] * s, y = py + DY[d] * s;
            while (in_bounds(x, y, b.cols, b.rows) && b.get(x, y) == v) {
                ++cnt;
                x += DX[d] * s;
                y += DY[d] * s;
            }
        }
        if (cnt >= b.win_len) return true;
    }
    return false;
}

// Score a single direction: count consecutive pieces and open ends
static int line_score(const Board &b, int x, int y,
                      int dx, int dy, int v, bool is_my) {
    int fwd = 0, bwd = 0;
    int nx = x + dx, ny = y + dy;
    while (in_bounds(nx, ny, b.cols, b.rows) && b.get(nx, ny) == v) {
        ++fwd;
        nx += dx;
        ny += dy;
    }
    nx = x - dx;
    ny = y - dy;
    while (in_bounds(nx, ny, b.cols, b.rows) && b.get(nx, ny) == v) {
        ++bwd;
        nx -= dx;
        ny -= dy;
    }

    int total = fwd + bwd + 1;
    if (total >= b.win_len) return WIN;

    // Check open ends (adjacent empty cells)
    bool of = in_bounds(x + dx*(fwd+1), y + dy*(fwd+1), b.cols, b.rows)
              && b.get(x + dx*(fwd+1), y + dy*(fwd+1)) == CV_NONE;
    bool ob = in_bounds(x - dx*(bwd+1), y - dy*(bwd+1), b.cols, b.rows)
              && b.get(x - dx*(bwd+1), y - dy*(bwd+1)) == CV_NONE;
    int oe = (of ? 1 : 0) + (ob ? 1 : 0);
    if (oe == 0) return 0;  // Both ends blocked - dead line

    // Look up score based on line length and open ends
    if (is_my) {
        switch (total) {
            case 4: return oe == 2 ? MY_OPEN4 : MY_HALF4;
            case 3: return oe == 2 ? MY_OPEN3 : MY_HALF3;
            case 2: return oe == 2 ? MY_OPEN2 : MY_HALF2;
            case 1: return oe == 2 ? MY_OPEN1 : MY_OPEN1 / 2;
        }
    } else {
        switch (total) {
            case 4: return oe == 2 ? OPP_OPEN4 : OPP_HALF4;
            case 3: return oe == 2 ? OPP_OPEN3 : OPP_HALF3;
            case 2: return oe == 2 ? OPP_OPEN2 : OPP_HALF2;
            case 1: return oe == 2 ? OPP_OPEN1 : OPP_OPEN1 / 2;
        }
    }
    return 0;
}

// Sum line scores across all 4 directions for a cell
static int cell_score(const Board &b, int x, int y, int v, bool is_my) {
    int t = 0;
    for (int d = 0; d < 4; d++)
        t += line_score(b, x, y, DX[d], DY[d], v, is_my);
    return t;
}

// Blend cell score with neighbors: (base + avg_neighbor) / 2
static int weighted_score(const Board &b, int x, int y,
                          const int *raw, int cols, int rows) {
    int base = raw[y * cols + x];
    int nsum = 0, ncnt = 0;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx, ny = y + dy;
            if (!in_bounds(nx, ny, cols, rows)) continue;
            if (b.get(nx, ny) != CV_NONE) continue;
            nsum += raw[ny * cols + nx];
            ncnt++;
        }
    if (ncnt == 0) return base;
    return (base + nsum / ncnt) / 2;
}

// Evaluate entire board - positive = good for me
static int eval_board(const Board &b) {
    // First pass: calculate raw scores for each empty cell
    int *raw = new int[b.size];
    for (int i = 0; i < b.size; i++) raw[i] = 0;
    for (int x = 0; x < b.cols; x++)
        for (int y = 0; y < b.rows; y++) {
            if (b.get(x, y) != CV_NONE) continue;
            raw[y * b.cols + x] = cell_score(b, x, y, CV_MY,  true)
                                 - cell_score(b, x, y, CV_OPP, false);
        }
    
    // Second pass: apply weighted scoring
    int score = 0;
    for (int x = 0; x < b.cols; x++)
        for (int y = 0; y < b.rows; y++) {
            if (b.get(x, y) != CV_NONE) continue;
            score += weighted_score(b, x, y, raw, b.cols, b.rows);
        }
    delete[] raw;
    return score;
}

struct Pt { int x, y; };
struct ScoredPt { int x, y, score; };

// Insertion sort for scored moves (descending order)
static void isort_desc(ScoredPt *a, int n) {
    for (int i = 1; i < n; i++) {
        ScoredPt k = a[i];
        int j = i - 1;
        while (j >= 0 && a[j].score < k.score) {
            a[j+1] = a[j];
            j--;
        }
        a[j+1] = k;
    }
}

static const int MAX_CANDS = 400;

// Generate candidate moves within radius of existing pieces
// Prunes far-away cells that are unlikely to be good moves
static int candidates(const Board &b, int radius, Pt *out) {
    bool *near = new bool[b.size];
    for (int i = 0; i < b.size; i++) near[i] = false;
    bool any = false;
    
    // Mark cells within radius of any existing piece
    for (int x = 0; x < b.cols; x++)
        for (int y = 0; y < b.rows; y++) {
            int cv = b.get(x, y);
            if (cv != CV_MY && cv != CV_OPP) continue;
            any = true;
            for (int dx = -radius; dx <= radius; dx++)
                for (int dy = -radius; dy <= radius; dy++) {
                    int nx = x + dx, ny = y + dy;
                    if (in_bounds(nx, ny, b.cols, b.rows) &&
                        b.get(nx, ny) == CV_NONE)
                        near[ny * b.cols + nx] = true;
                }
        }
    
    int cnt = 0;
    // Empty board - return center
    if (!any) {
        int cx = b.cols/2, cy = b.rows/2, bd = INT_MAX, bx = -1, by = -1;
        for (int x = 0; x < b.cols; x++)
            for (int y = 0; y < b.rows; y++) {
                if (b.get(x, y) != CV_NONE) continue;
                int d = (x-cx)*(x-cx)+(y-cy)*(y-cy);
                if (d < bd) { bd = d; bx = x; by = y; }
            }
        delete[] near;
        if (bx != -1) out[cnt++] = {bx, by};
        return cnt;
    }
    
    // Collect all marked cells
    for (int x = 0; x < b.cols; x++)
        for (int y = 0; y < b.rows; y++)
            if (near[y * b.cols + x]) out[cnt++] = {x, y};
    delete[] near;
    return cnt;
}

static const int MAX_BRANCH = 10;

// Alpha-beta minimax with move ordering and pruning
static int alphabeta(Board &b, int depth, int alpha, int beta, bool maxi) {
    if (depth == 0) return eval_board(b);  // Leaf node

    Pt *rc = new Pt[MAX_CANDS];
    int n = candidates(b, 2, rc);
    if (n == 0) { delete[] rc; return eval_board(b); }

    // Sort moves by estimated quality for better pruning
    int mv = maxi ? CV_MY : CV_OPP;
    ScoredPt *sc = new ScoredPt[n];
    for (int i = 0; i < n; i++)
        sc[i] = { rc[i].x, rc[i].y,
                  cell_score(b, rc[i].x, rc[i].y, CV_MY,  true) +
                  cell_score(b, rc[i].x, rc[i].y, CV_OPP, false) };
    isort_desc(sc, n);
    if (n > MAX_BRANCH) n = MAX_BRANCH;
    delete[] rc;

    int best = maxi ? INT_MIN : INT_MAX;
    for (int i = 0; i < n; i++) {
        int x = sc[i].x, y = sc[i].y;
        b.set(x, y, mv);
        int score = check_win(b, x, y, mv)
                    ? (maxi ? WIN + depth : -(WIN + depth))
                    : alphabeta(b, depth - 1, alpha, beta, !maxi);
        b.set(x, y, CV_NONE);

        if (maxi) {
            if (score > best) best = score;
            if (best > alpha) alpha = best;
        } else {
            if (score < best) best = score;
            if (best < beta) beta = best;
        }
        if (alpha >= beta) break;  // Prune remaining branches
    }
    delete[] sc;
    return best;
}

// Main decision function
game::Point MyPlayer::make_move(const game::State &state) {
    int cols    = state.get_opts().cols;
    int rows    = state.get_opts().rows;
    int win_len = state.get_opts().win_len;
    Sign opp    = (m_sign == Sign::X) ? Sign::O : Sign::X;

    // Copy game state to local board for minimax
    Board b;
    b.alloc(cols, rows, win_len);
    for (int x = 0; x < cols; x++)
        for (int y = 0; y < rows; y++) {
            Sign v = state.get_value(x, y);
            if      (v == m_sign)     b.set(x, y, CV_MY);
            else if (v == opp)        b.set(x, y, CV_OPP);
            else if (v != Sign::NONE) b.set(x, y, CV_OBSTACLE);
        }

    // Precompute raw scores for root move ordering
    int *raw = new int[cols * rows];
    for (int i = 0; i < cols*rows; i++) raw[i] = 0;
    for (int x = 0; x < cols; x++)
        for (int y = 0; y < rows; y++) {
            if (b.get(x, y) != CV_NONE) continue;
            raw[y*cols+x] = cell_score(b, x, y, CV_MY,  true)
                          + cell_score(b, x, y, CV_OPP, false);
        }

    // Generate root candidates (radius 2 for wider search)
    Pt *cands = new Pt[MAX_CANDS];
    int n = candidates(b, 2, cands);
    if (n == 0)
        for (int x = 0; x < cols && n < MAX_CANDS; x++)
            for (int y = 0; y < rows && n < MAX_CANDS; y++)
                if (b.get(x, y) == CV_NONE) cands[n++] = {x, y};
    if (n == 0) { delete[] raw; delete[] cands; b.free_mem(); return {0,0}; }

    // Priority 1: Immediate win
    for (int i = 0; i < n; i++) {
        int x = cands[i].x, y = cands[i].y;
        b.set(x, y, CV_MY);
        bool win = check_win(b, x, y, CV_MY);
        b.set(x, y, CV_NONE);
        if (win) {
            game::Point r{x,y};
            delete[] raw; delete[] cands; b.free_mem();
            return r;
        }
    }

    // Priority 2: Block opponent's immediate win
    int bx = -1, by = -1;
    for (int i = 0; i < n; i++) {
        int x = cands[i].x, y = cands[i].y;
        b.set(x, y, CV_OPP);
        bool win = check_win(b, x, y, CV_OPP);
        b.set(x, y, CV_NONE);
        if (win) { bx = x; by = y; break; }
    }

    // Priority 3: Minimax search (depth 3)
    ScoredPt *sc = new ScoredPt[n];
    for (int i = 0; i < n; i++)
        sc[i] = { cands[i].x, cands[i].y,
                  weighted_score(b, cands[i].x, cands[i].y, raw, cols, rows) };
    isort_desc(sc, n);
    
    static const int ROOT_BRANCH = 15;
    static const int DEPTH = 3;
    if (n > ROOT_BRANCH) n = ROOT_BRANCH;
    delete[] cands;
    delete[] raw;

    // Evaluate top candidates with minimax
    game::Point best_move{sc[0].x, sc[0].y};
    int best_score = INT_MIN;
    for (int i = 0; i < n; i++) {
        int x = sc[i].x, y = sc[i].y;
        b.set(x, y, CV_MY);
        int score = check_win(b, x, y, CV_MY)
                    ? WIN + DEPTH
                    : alphabeta(b, DEPTH - 1, INT_MIN, INT_MAX, false);
        b.set(x, y, CV_NONE);
        if (score > best_score) {
            best_score = score;
            best_move = {x, y};
        }
    }
    delete[] sc;
    b.free_mem();

    // Fallback: block opponent's win only if losing badly
    if (bx != -1 && best_score < -(WIN / 2))
        return {bx, by};
    return best_move;
}

} // namespace ttt::my_player
