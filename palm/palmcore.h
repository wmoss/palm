#ifndef PALMCORE_H
#define PALMCORE_H

typedef enum {
    pbf_type_varint = 0,
    pbf_type_fixed64 = 1,
    pbf_type_length = 2,
    pbf_type_fixed32 = 5
} pbf_field_type;

typedef struct pbf_mark {
    char exists;
    pbf_field_type ftype;
    union {
        uint64_t i64;
        uint32_t i32;
        struct {
            uint64_t len;
            char *data;
        } s;
    } fdata;
    unsigned char *raw;
    uint64_t raw_len; // copy if nonzero
    unsigned char buf[20]; // for new ints or headers
    unsigned char buf_len; // copy if nonzero
} pbf_mark;

typedef struct pbf_protobuf {
    unsigned char *data;
    uint64_t data_length;
    pbf_mark *marks;
    int num_marks;
    int max_mark;
    int parsed;
} pbf_protobuf;
pbf_protobuf * pbf_load(char *data, uint64_t size);

void pbf_free(pbf_protobuf *pbf);

int pbf_get_bytes(pbf_protobuf *pbf, uint64_t field_num,
    char **out, uint64_t *length);

int pbf_get_integer(pbf_protobuf *pbf, uint64_t field_num, uint64_t *res);

int pbf_get_signed_integer(pbf_protobuf *pbf,
    uint64_t field_num, int64_t *res,
    int32_t *res32, int use_zigzag);
int pbf_exists(pbf_protobuf *pbf, uint64_t field_num);

int pbf_set_bytes(pbf_protobuf *pbf, uint64_t field_num,
        char *out, uint64_t length);

int pbf_set_integer(pbf_protobuf *pbf, uint64_t field_num,
        uint64_t value, int fixed);

int pbf_set_signed_integer(pbf_protobuf *pbf, uint64_t field_num,
        int64_t value, int zigzag);

unsigned char *pbf_serialize(pbf_protobuf *pbf, int *length);

#endif /* PALMCORE_H */