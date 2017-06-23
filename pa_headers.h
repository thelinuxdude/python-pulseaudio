#ifndef __SIMPLE_A_H__
#define __SIMPLE_A_H__

//#define __SIZE_TYPE__ int
#include <pulse/pulseaudio.h>

#ifdef HAVE_STD_BOOL
typedef bool pa_bool_t;
#else
typedef int pa_bool_t;
#endif

#define PA_MAX_WRITE_INDEX_CORRECTIONS 32
#define PA_ENCODING_MAX 7
#define PA_MAX_FORMATS (PA_ENCODING_MAX)
#define HISTORY_MAX 64

/* The head of the linked list. Use this in the structure that shall
 * contain the head of the linked list */
#define PA_LLIST_HEAD(t,name)                                           \
    t *name

/* The pointers in the linked list's items. Use this in the item structure */
#define PA_LLIST_FIELDS(t)                                              \
    t *next, *prev

typedef struct pa_atomic {
	size_t value;
} pa_atomic_t;

struct pa_memblock {
};

struct pa_memblockq {
};

struct pa_smoother {
};

#define PA_REFCNT_DECLARE \
    pa_atomic_t _ref

#define PA_REFCNT_VALUE(p) \
    pa_atomic_load(&(p)->_ref)

#define PA_REFCNT_INIT_ZERO(p) \
    pa_atomic_store(&(p)->_ref, 0)

/* A memchunk describes a part of a memblock. In contrast to the memblock, a
 * memchunk is not allocated dynamically or reference counted, instead
 * it is usually stored on the stack and copied around */

struct pa_memchunk {
    pa_memblock *memblock;
    size_t index, length;
};

typedef struct pa_index_correction {
    uint32_t tag;
    int64_t value;
    pa_bool_t valid:1;
    pa_bool_t absolute:1;
    pa_bool_t corrupt:1;
} pa_index_correction;


struct pa_stream {
    PA_REFCNT_DECLARE;
    PA_LLIST_FIELDS(pa_stream);

    pa_context *context;
    pa_mainloop_api *mainloop;

    uint32_t direct_on_input;

    pa_stream_direction_t direction;
    pa_stream_state_t state;
    pa_stream_flags_t flags;

    pa_sample_spec sample_spec;
    pa_channel_map channel_map;
    uint8_t n_formats;
    pa_format_info *req_formats[PA_MAX_FORMATS];
    pa_format_info *format;

    pa_proplist *proplist;

    pa_bool_t channel_valid:1;
    pa_bool_t suspended:1;
    pa_bool_t corked:1;
    pa_bool_t timing_info_valid:1;
    pa_bool_t auto_timing_update_requested:1;

    uint32_t channel;
    uint32_t syncid;
    uint32_t stream_index;

    int64_t requested_bytes;
    pa_buffer_attr buffer_attr;

    uint32_t device_index;
    char *device_name;

    /* playback */
    pa_memblock *write_memblock;
    void *write_data;
    int64_t latest_underrun_at_index;

    /* recording */
    pa_memchunk peek_memchunk;
    void *peek_data;
    pa_memblockq *record_memblockq;

    /* Store latest latency info */
    pa_timing_info timing_info;

    /* Use to make sure that time advances monotonically */
    pa_usec_t previous_time;

    /* time updates with tags older than these are invalid */
    uint32_t write_index_not_before;
    uint32_t read_index_not_before;

    /* Data about individual timing update corrections */
    pa_index_correction write_index_corrections[PA_MAX_WRITE_INDEX_CORRECTIONS];
    int current_write_index_correction;

    /* Latency interpolation stuff */
    pa_time_event *auto_timing_update_event;
    pa_usec_t auto_timing_interval_usec;

    pa_smoother *smoother;

    /* Callbacks */
    pa_stream_notify_cb_t state_callback;
    void *state_userdata;
    pa_stream_request_cb_t read_callback;
    void *read_userdata;
    pa_stream_request_cb_t write_callback;
    void *write_userdata;
    pa_stream_notify_cb_t overflow_callback;
    void *overflow_userdata;
    pa_stream_notify_cb_t underflow_callback;
    void *underflow_userdata;
    pa_stream_notify_cb_t latency_update_callback;
    void *latency_update_userdata;
    pa_stream_notify_cb_t moved_callback;
    void *moved_userdata;
    pa_stream_notify_cb_t suspended_callback;
    void *suspended_userdata;
    pa_stream_notify_cb_t started_callback;
    void *started_userdata;
    pa_stream_event_cb_t event_callback;
    void *event_userdata;
    pa_stream_notify_cb_t buffer_attr_callback;
    void *buffer_attr_userdata;
};


struct pa_simple {
    pa_threaded_mainloop *mainloop;
    pa_context *context;
    pa_stream *stream;
    pa_stream_direction_t direction;

    const void *read_data;
    size_t read_index, read_length;

    int operation_success;
};

#if 0
/** A callback for operation state changes */
typedef void (*pa_operation_notify_cb_t) (pa_operation *o, void *userdata);
typedef void (*pa_operation_cb_t)(void);

typedef struct pa_socket_client pa_socket_client;
typedef struct pa_iochannel pa_iochannel;

typedef void (*pa_socket_client_cb_t)(pa_socket_client *c, pa_iochannel*io, void *userdata);

struct pa_operation {
    PA_REFCNT_DECLARE;

    pa_context *context;
    pa_stream *stream;

    PA_LLIST_FIELDS(pa_operation);

    pa_operation_state_t state;
    void *userdata;
    pa_operation_cb_t callback;
    void *state_userdata;
    pa_operation_notify_cb_t state_callback;

    void *priv; /* some operations might need this */
};

struct pa_socket_client {
    PA_REFCNT_DECLARE;
    int fd;

    pa_mainloop_api *mainloop;
    pa_io_event *io_event;
    pa_time_event *timeout_event;
    pa_defer_event *defer_event;

    pa_socket_client_cb_t callback;
    void *userdata;

    pa_bool_t local;

#ifdef HAVE_LIBASYNCNS
    asyncns_t *asyncns;
    asyncns_query_t * asyncns_query;
    pa_io_event *asyncns_io_event;
#endif
};



struct pa_pstream {

};

struct pa_pdispatch {

};

struct pa_hashmap {

};

struct pa_mempool {

};

struct pa_strlist {

};

struct pa_client_conf {

};

struct pa_dbus_wrap_connection {

};

typedef struct pa_context pa_context;

/** Callback prototype for pa_ext_device_manager_set_subscribe_cb(). \since 0.9.21 */
typedef void (*pa_ext_device_manager_subscribe_cb_t)(
        pa_context *c,
        void *userdata);

/** Callback prototype for pa_ext_device_restore_set_subscribe_cb(). \since 1.0 */
typedef void (*pa_ext_device_restore_subscribe_cb_t)(
        pa_context *c,
        pa_device_type_t type,
        uint32_t idx,
        void *userdata);

/** Callback prototype for pa_ext_stream_restore_set_subscribe_cb(). \since 0.9.12 */
typedef void (*pa_ext_stream_restore_subscribe_cb_t)(
        pa_context *c,
        void *userdata);


struct pa_context {
    PA_REFCNT_DECLARE;

#ifdef HAVE_DBUS
    pa_dbus_wrap_connection *system_bus;
    pa_dbus_wrap_connection *session_bus;
#endif

    pa_proplist *proplist;
    pa_mainloop_api* mainloop;

    pa_socket_client *client;
    pa_pstream *pstream;
    pa_pdispatch *pdispatch;

    pa_hashmap *record_streams, *playback_streams;
    PA_LLIST_HEAD(pa_stream, streams);
    PA_LLIST_HEAD(pa_operation, operations);

    uint32_t version;
    uint32_t ctag;
    uint32_t csyncid;
    int error;
    pa_context_state_t state;

    pa_context_notify_cb_t state_callback;
    void *state_userdata;
    pa_context_subscribe_cb_t subscribe_callback;
    void *subscribe_userdata;
    pa_context_event_cb_t event_callback;
    void *event_userdata;

    pa_mempool *mempool;

    pa_bool_t is_local:1;
    pa_bool_t do_shm:1;
    pa_bool_t server_specified:1;
    pa_bool_t no_fail:1;
    pa_bool_t do_autospawn:1;
    pa_bool_t use_rtclock:1;
    pa_bool_t filter_added:1;
    pa_spawn_api spawn_api;

    pa_strlist *server_list;

    char *server;

    pa_client_conf *conf;

    uint32_t client_index;

    /* Extension specific data */
    struct {
        pa_ext_device_manager_subscribe_cb_t callback;
        void *userdata;
    } ext_device_manager;
    struct {
        pa_ext_device_restore_subscribe_cb_t callback;
        void *userdata;
    } ext_device_restore;
    struct {
        pa_ext_stream_restore_subscribe_cb_t callback;
        void *userdata;
    } ext_stream_restore;
};
#endif

#endif
