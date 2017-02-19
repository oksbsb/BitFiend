#ifndef TRACKER_ANNOUNCE_H
#define TRACKER_ANNOUNCE_H

#include "list.h"
#include <stddef.h>
#include <netinet/ip.h>

typedef enum {
    TORRENT_EVENT_STARTED,
    TORRENT_EVENT_COMPLETED,
    TORRENT_EVENT_STOPPED
}torrent_event_t;

enum {
    REQUEST_HAS_IP                  = (1 << 0),
    REQUEST_HAS_NUMWANT             = (1 << 1),
    REQUEST_HAS_NO_PEER_ID          = (1 << 2),
    REQUEST_HAS_COMPACT             = (1 << 3),
    REQUEST_HAS_KEY                 = (1 << 4),
    REQUEST_HAS_TRACKER_ID          = (1 << 5),
    REQUEST_HAS_EVENT               = (1 << 6)
};

enum {
    RESPONSE_HAS_FAILURE_REASON     = (1 << 0),
    RESPONSE_HAS_WARNING_MESSAGE    = (1 << 1),
    RESPONSE_HAS_MIN_INTERVAL       = (1 << 2)
};

#define SET_HAS(_ptr, _has) ((_ptr)->has |= (_has))
#define CLR_HAS(_ptr, _has) ((_ptr)->has &= ~(_has))
#define HAS(_ptr, _has) !!((_ptr)->has & (_has))

typedef struct tracker_announce_request {
    char has;
    char info_hash[20]; 
    char peer_id[20];
    struct sockaddr_in ip;
    uint16_t port;
    unsigned long uploaded;
    unsigned long downloaded;  
    unsigned long left;
    torrent_event_t event;
    unsigned numwant;
    bool no_peer_id;
    bool compact;
    char *key;
    char *tracker_id;
}tracker_announce_request_t;

typedef struct tracker_announce_resp {
    char has;
    int64_t interval;
    char *tracker_id;
    int64_t complete;
    int64_t incomplete;
    list_t *peers;
    char *failure_reason;
    char *warning_message;
    int64_t min_interval;
}tracker_announce_resp_t;

typedef struct peer{
    char peer_id[20];
    struct sockaddr_in ip;
}peer_t;

int tracker_announce(const char *url, tracker_announce_request_t *request);

#endif