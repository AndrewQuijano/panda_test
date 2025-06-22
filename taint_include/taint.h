#ifndef __TAINT_H__
#define __TAINT_H__

#if !defined(KERN_MOD)
    #include <stdlib.h>
    #include <stdint.h>
    #include <string.h>
#else
    #include <linux/types.h>
    #include <linux/string.h>
#endif

#include "hypercall.h"

static const int ENABLE_TAINT = 6;
static const int LABEL_BUFFER = 7;
static const int LABEL_BUFFER_POS = 8;
static const int QUERY_BUFFER = 9;
static const int LABEL_REGISTER = 10;
static const int QUERY_REGISTER = 11;
static const int LOG = 12;

static inline
void hypercall_log(char *c_str) {
    igloo_hypercall4(LOG, strlen(c_str)+1, (unsigned long) c_str, 0, 0);
}

static inline
void hypercall_query_reg(uint32_t reg_num, uint32_t reg_off, long label, long positive) {
    igloo_hypercall4(QUERY_REGISTER, reg_num, reg_off, positive, label);
}

static inline
void hypercall_label_reg(uint32_t reg_num, uint32_t reg_off, long label) {
    igloo_hypercall4(LABEL_REGISTER, reg_num, reg_off, 0, label);
}

static inline
void hypercall_enable_taint(void) {
    igloo_hypercall4(ENABLE_TAINT, 0, 0, 0, 0);
}

static inline
void hypercall_label_buffer(void *buf, unsigned long len, long label) {
    igloo_hypercall4(LABEL_BUFFER, (unsigned long) buf, len, 0, label);
}

static inline
void hypercall_query_buffer(void *buf, uint32_t off, long label, long positive) {
    igloo_hypercall4(QUERY_BUFFER, (unsigned long) buf, off, positive, label);
}

/* buf is the address of the buffer to be labeled
 *  * label is the label to be applied to the buffer
 *   * len is the length of the buffer to be labeled */
static inline
void panda_taint_label_buffer(void *buf, int label, unsigned long len) {
    hypercall_label_buffer(buf, len, label);
}

static inline
void panda_taint_query_buffer(void *buf, unsigned long off, long label, long positive) {
    hypercall_query_buffer(buf, off, label, positive);
}

static inline
void panda_taint_assert_label_found(void *buf, uint32_t off, uint32_t expected_label) {
    panda_taint_query_buffer(buf, off, expected_label, 1);
}

static inline
void panda_taint_assert_label_not_found(void *buf, uint32_t off, uint32_t expected_label) {
    panda_taint_query_buffer(buf, off, expected_label, 0);
}

static inline
void panda_taint_assert_label_found_range(void *buf, size_t len, uint32_t expected_label) {
    size_t i;
    for(i=0;i<len;i++) {
        panda_taint_assert_label_found(buf, i, expected_label);
    }
}

static inline
void panda_taint_assert_label_not_found_range(void *buf, size_t len, uint32_t expected_label) {
    int i;
    for(i=0;i<len;i++) {
        panda_taint_assert_label_not_found(buf, i, expected_label);
    }
}

static inline
void panda_taint_log(char *c_str) {
    hypercall_log(c_str);
}

#endif // __TAINT_H__
