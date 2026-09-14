#include "context.h"
#include "assert.h"
#include "stdlib.h"
#include "string.h"

__generator __main_gen;
__generator* __now_gen = &__main_gen;

// Task 2

void __err_stk_push(__ctx* ctx){
    assert(ctx != 0);

    __err_stk_node* node = (__err_stk_node*) malloc(sizeof(*node));
    assert(node != 0);
    node->ctx = ctx;
    node->prev = __now_gen->__err_stk_head;
    __now_gen->__err_stk_head = node;
}

__ctx* __err_stk_pop(){
    assert(__now_gen->__err_stk_head != 0);

    __err_stk_node* node = __now_gen->__err_stk_head;
    __ctx* ctx = node->ctx;
    __now_gen->__err_stk_head = node->prev;
    free(node);
    return ctx;
}

void __err_cleanup(const int* error){
    if(!*error) __err_stk_pop();
}

int __check_err_stk(){
    return __now_gen->__err_stk_head == 0;
}

int send(__generator* gen, int value) {
    if (gen == 0) throw(ERR_GENNIL);
    gen->data = value;
    gen->caller=__now_gen;
    int is_restore = __ctx_save(&__now_gen->ctx);
    if(!is_restore){
        __now_gen=gen;
        __ctx_restore(&gen->ctx, 1);
    }
    return gen->data;
}

int yield(int value) {
    if (__now_gen->caller == 0) throw(ERR_GENNIL);
    __now_gen->data = value;
    int is_restore =__ctx_save(&__now_gen->ctx);
    if(!is_restore){
        __now_gen=__now_gen->caller;
        __ctx_restore(&__now_gen->ctx, 1);
    }
    return __now_gen->data;
}

void back_to_reality() {
    throw(ERR_GENEND);
}

// Task 3

__generator* generator(void (*f)(int), int arg) {
    __generator * new_gen = (__generator *) malloc(sizeof(__generator));
    size_t size = 8192;
    new_gen->f = f;
    new_gen->stack = malloc(size);
    new_gen->__err_stk_head = NULL;
    long long* ctx_addr = (void*)&new_gen->ctx;
    long long* stack_addr = (void*)new_gen->stack;
    ctx_addr[6] = (long long)(stack_addr + size / sizeof(long long) - 1);
    ctx_addr[7] = (long long)(void*)f;
    ctx_addr[8] = arg;
    stack_addr[size / sizeof(long long) - 1] =
        (long long)(void*)back_to_reality;
    new_gen->caller = NULL;
    new_gen->data = 0;

    return new_gen;
}

void generator_free(__generator** gen) {
    if (*gen == NULL) throw(ERR_GENNIL);
    free((*gen)->stack);
    while ((*gen)->__err_stk_head != 0) {
        __err_stk_node* tmp = (*gen)->__err_stk_head;
        (*gen)->__err_stk_head = (*gen)->__err_stk_head->prev;
        free(tmp->ctx);
        free(tmp);
    }
    free(*gen);
    *gen = NULL;
}