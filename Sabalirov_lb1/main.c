#include "stdio.h"
#include "malloc.h"

#define MAX_ASSESMENT 44
#define MAX_N 40

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define GET_SQUARE_ROW(info, x, side) (((info.R >> (info.n - side)) << (info.n - x - side)))


typedef struct {
    size_t x;
    size_t y;
    size_t side;
} square_t;

typedef struct {
    size_t N;
    size_t square;
    size_t R;
    size_t n;
} info_t;

typedef struct {
    size_t curr_square;
    size_t matrix[MAX_N];
    square_t squares[MAX_ASSESMENT];
    size_t len;
} combination_t;


int get_best_limit(size_t N, size_t* best_side, size_t* is_prime){
    *is_prime = 0;

    if (N % 2 == 0){
        *best_side = N / 2;
        return 4;
    }

    for(size_t i = 3; (i * i) <= N; i += 2){
        if (N % i == 0){
            size_t j = N / i;
            *best_side = j*(i / 2 + 1);
            size_t low_side = *best_side - j;
            return ((*best_side) / low_side)*2 + 2 + (low_side / (*best_side - low_side))*2;
        }
    }

    *is_prime = 1;
    *best_side = N / 2 + 1;

    size_t assessments[6] = {N + 4, N / 2 + 7, N / 3 + 9, N / 4 + 10, N / 5 + 11, N / 6 + 12};
    size_t assessment = assessments[0];
    for (size_t i = 1; i < 6; ++i)
        assessment = MIN(assessment, assessments[i]); 
    return MAX(assessment - 3, 6);
}


int can_place(size_t row, info_t info, size_t x, size_t side){
    size_t b = (row) ^ (((info.R >> (info.n - side)) << (info.n - x - side)));
    size_t c = (row) | (((info.R >> (info.n - side)) << (info.n - x - side)));
    if (b == c)
        return 1;
    return 0;
}


void print_square(square_t square, size_t correction_x, size_t correction_y){
    printf("%ld %ld %ld\n", square.x + correction_x, square.y + correction_y, square.side);
}


void output_solution(square_t* squares, size_t len, size_t delta_n){
    printf("%ld\n", len);
    for (size_t j = 0; j < 3; ++j)
        print_square(squares[j], 1, 1);
    for (size_t j = 3; j < len; ++j)
        print_square(squares[j], delta_n + 1, delta_n + 1);
}


void solution(int N){
    size_t best_side, is_prime;
    size_t limit = get_best_limit(N, &best_side, &is_prime);
    info_t info = {N, N*N, (2 << (best_side - 1)) - 1, best_side};
    square_t pants[3] = {
        (square_t){0, 0, best_side},
        (square_t){0, best_side, N - best_side},
        (square_t){best_side, 0, N - best_side}
    };
    size_t init_square = best_side*best_side + 2*(N - best_side)*(N - best_side);
    size_t init_conditions = 2*best_side - N;
    size_t side_assessment = (N - best_side) + 1;
    while (1){
        combination_t* stack = calloc(N*N, sizeof(combination_t));
        for (size_t i = 0; i < 3; ++i)
            stack[0].squares[i] = pants[i];
        for (size_t y = 0; y < init_conditions; ++y)
            stack[0].matrix[y] |= GET_SQUARE_ROW(info, 0, init_conditions);
        stack[0].len = 3;
        stack[0].curr_square = init_square;
        int top = 0;
        while (top >= 0){
            combination_t curr_comb = stack[top--];
            if (curr_comb.curr_square == info.square){
                output_solution(curr_comb.squares, curr_comb.len, (N - best_side));
                return;
            }

            if (curr_comb.len == limit)
                continue;

            combination_t comb = curr_comb;
            size_t max_square = info.square - comb.curr_square;
            for (int y = 0; y <= info.n - 1; ++y){
                for (int x = 0; x <= info.n - 1; ++x){
                    for (size_t side = side_assessment; side > 0; --side){
                        if (x + side <= info.n && y + side <= info.n && can_place(curr_comb.matrix[y], info, x, side)){
                            for (size_t s = 1; s <= side; ++s){
                                comb.squares[comb.len++] = (square_t){x, y, s};
                                comb.curr_square += s*s;
                                for (size_t i = y; i < s + y; ++i)
                                    comb.matrix[i] |= GET_SQUARE_ROW(info, x, s);
                                stack[++top] = comb;
                                comb = curr_comb;
                            }
                            goto next;
                        }
                    }
                }
            }
            next:;
        }
        ++limit;
        free(stack);
    }

}


int main(){
    size_t N;
    scanf("%ld", &N);   
    solution(N);
    return 0;
}