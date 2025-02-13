#include "stdio.h"
#include "malloc.h"
#include "string.h"

#define MAX_ASSESMENT 44
#define MAX_N 40
#define ASSESSMENT_MISTAKE 3
#define ASSESSMENTS_COUNT 6
#define ASSESSMENT_ARR {N + 4, N / 2 + 7, N / 3 + 9, N / 4 + 10, N / 5 + 11, N / 6 + 12}

#define MIN(a, b) (a < b ? a : b)
#define MAX(a, b) (a > b ? a : b)
#define INIT_FOR(y1, y2, x) (y1 == y2 ? x : 0)
#define GET_SQUARE_ROW(info, x, side) ((info.R >> (info.n - side)) << (info.n - x - side))


typedef struct {
    size_t x;
    size_t y;
    size_t side;
} square_t;

typedef struct {
    const size_t N;
    const size_t square;
    const size_t R;
    const size_t n;
} info_t;

typedef struct {
    size_t curr_square;
    size_t matrix[MAX_N];
    square_t squares[MAX_ASSESMENT];
    size_t len;
    size_t next_x;
    size_t next_y;
} combination_t;


size_t get_best_limit(size_t N, size_t* best_side){
    if (N % 2 == 0){
        *best_side = N / 2;
        return 4;
    }

    for(size_t i = 3; (i * i) <= N; i += 2){
        if (N % i == 0){
            size_t j = N / i;
            *best_side = j*(i / 2 + 1);
            const size_t low_side = *best_side - j;
            return ((*best_side) / low_side)*2 + 2 + (low_side / (*best_side - low_side))*2;
        }
    }

    *best_side = N / 2 + 1;

    const size_t assessments[ASSESSMENTS_COUNT] = ASSESSMENT_ARR;
    size_t assessment = assessments[0];
    for (size_t i = 1; i < ASSESSMENTS_COUNT; ++i)
        assessment = MIN(assessment, assessments[i]); 
    return assessment - ASSESSMENT_MISTAKE;
}


int can_place(size_t row, info_t info, size_t x, size_t side){
    size_t square = GET_SQUARE_ROW(info, x, side);
    return (row | square) == (row ^ square);
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


void solution(const size_t N){
    size_t best_side, square;
    size_t limit = get_best_limit(N, &best_side);
    const info_t info = {N, N*N, (2 << (best_side - 1)) - 1, best_side};
    const size_t init_square = best_side*best_side + 2*(N - best_side)*(N - best_side);
    const size_t init_conditions_side = 2*best_side - N;
    const size_t init_conditions = GET_SQUARE_ROW(info, 0, init_conditions_side);
    const square_t pants[3] = {
        (const square_t){0, 0, best_side},
        (const square_t){0, best_side, N - best_side},
        (const square_t){best_side, 0, N - best_side}
    };
    combination_t curr_comb, comb;
    const size_t mem_size = N*N;
    combination_t* stack = calloc(mem_size, sizeof(combination_t));
    int top;
    while (1){
        for (size_t i = 0; i < 3; ++i)
            stack[0].squares[i] = pants[i];
        for (size_t y = 0; y < init_conditions_side; ++y)
            stack[0].matrix[y] |= init_conditions;
        stack[0].len = 3;
        stack[0].curr_square = init_square;
        stack[0].next_x = init_conditions_side;
        stack[0].next_y = 0;

        top = 0;
        while (top >= 0){
            curr_comb = stack[top--];
            if (curr_comb.curr_square == info.square){
                output_solution(curr_comb.squares, curr_comb.len, (N - best_side));
                return;
            }

            if (curr_comb.len == limit)
                continue;

            comb = curr_comb;
            for (size_t y = comb.next_y; y < info.n; ++y){
                for (size_t x = INIT_FOR(y, comb.next_y, comb.next_x); x < info.n; ++x){
                    for (size_t max_side = info.n - MAX(x, y); max_side > 0; --max_side){
                        if (can_place(curr_comb.matrix[y], info, x, max_side)){
                            for (size_t side = 1; side <= max_side; ++side){
                                comb.squares[comb.len++] = (square_t){x, y, side};
                                comb.curr_square += side*side;
                                square = GET_SQUARE_ROW(info, x, side);
                                for (size_t i = y; i < side + y; ++i)
                                    comb.matrix[i] |= square;
                                comb.next_x = x + 1;
                                comb.next_y = y;
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
        memset(stack, 0, mem_size*sizeof(combination_t));
    }

}


int main(){
    size_t N;
    scanf("%ld", &N);   
    solution(N);
    return 0;
}
