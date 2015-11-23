/*
 * librdkafka - Apache Kafka C/C++ library
 *
 * Copyright (c) 2014 Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

/**
 * Apache Kafka consumer & producer
 *
 * rdkafkacpp.h contains the public C++ API for librdkafka.
 * The API isdocumented in this file as comments prefixing the class, function,
 * type, enum, define, etc.
 * For more information, see the C interface in rdkafka.h and read the
 * manual in INTRODUCTION.md.
 * The C++ interface is STD C++ '03 compliant.
 */

#include <string>
#include <list>
#include <vector>
#include <stdint.h>


#ifdef _MSC_VER
#undef RD_EXPORT
#ifdef LIBRDKAFKACPP_EXPORTS
#define RD_EXPORT __declspec(dllexport)
#else
#define RD_EXPORT __declspec(dllimport)
#endif
#else
#define RD_EXPORT
#endif

namespace RdKafka {

/**
 * librdkafka version
 *
 * Interpreted as hex MM.mm.rr.xx:
 *   MM = Major
 *   mm = minor
 *   rr = revision
 *   xx = pre-release id (0xff is the final release)
 *
 * I.e.: 0x000801ff = 0.8.1
 */
#define RD_KAFKA_VERSION  0x00090001

/**
 * Returns the librdkafka version as integer.
 */
RD_EXPORT
int          version ();

/**
 * Returns the librdkafka version as string.
 */
RD_EXPORT
std::string  version_str();

/**
 * Returns a CSV list of the supported debug contexts
 * for use with Conf::Set("debug", ..).
 */
RD_EXPORT
std::string get_debug_contexts();

/**
 * ErrorCode
 * Returned throughout the library to signal errors and events.
 */
enum ErrorCode {
  ERR__BEGIN = -200,             /* begin internal error codes */
  ERR__BAD_MSG = -199,           /* Received message is incorrect */
  ERR__BAD_COMPRESSION = -198,   /* Bad/unknown compression */
  ERR__DESTROY = -197,           /* Broker is going away */
  ERR__FAIL = -196,              /* Generic failure */
  ERR__TRANSPORT = -195,         /* Broker transport error */
  ERR__CRIT_SYS_RESOURCE = -194, /* Critical system resource
                                  * failure */
  ERR__RESOLVE = -193,           /* Failed to resolve broker */
  ERR__MSG_TIMED_OUT = -192,     /* Produced message timed out*/
  ERR__PARTITION_EOF = -191,     /* Reached the end of the
                                  * topic+partition queue on
                                  * the broker.
                                  * Not really an error. */
  ERR__UNKNOWN_PARTITION = -190, /* Permanent:
                                  * Partition does not
                                  * exist in cluster. */
  ERR__FS = -189,                /* File or filesystem error */
  ERR__UNKNOWN_TOPIC = -188,     /* Permanent:
                                  * Topic does not exist
                                  * in cluster. */
  ERR__ALL_BROKERS_DOWN = -187,  /* All broker connections
                                  * are down. */
  ERR__INVALID_ARG = -186,       /* Invalid argument, or
                                  * invalid configuration */
  ERR__TIMED_OUT = -185,         /* Operation timed out */
  ERR__QUEUE_FULL = -184,        /* Queue is full */
  ERR__ISR_INSUFF = -183,        /* ISR count < required.acks */
  ERR__NODE_UPDATE = -182,       /* Broker node update */
  ERR__SSL = -181,               /* SSL certification verification failed */
  ERR__WAIT_COORD = -180,        /* Waiting for coordinator
                                  * to become available. */
  ERR__UNKNOWN_GROUP = -179,     /* Unknown client group */
  ERR__IN_PROGRESS = -178,       /* Operation in progress */
  ERR__PREV_IN_PROGRESS = -177,  /* Previous operation
                                  * in progress, wait for
                                  * it to finish. */
  ERR__EXISTING_SUBSCRIPTION = -176, /* This operation
                                      * would interfer
                                      * with an existing
                                      * subscription */
  ERR__ASSIGN_PARTITIONS = -175, /* For use w rebalance_cb*/
  ERR__REVOKE_PARTITIONS = -174, /* For use w rebalance_cb*/
  ERR__CONFLICT = -173,          /* Conflicting use */
  ERR__STATE = -172,             /* Wrong state */
  ERR__UNKNOWN_PROTOCOL = -171,  /* Unknown protocol */
  ERR__NOT_IMPLEMENTED = -170,   /* Not implemented */
  ERR__AUTHENTICATION = -169,    /* Authentication failure */
  ERR__END = -100,               /* end internal error codes */

  /* Standard Kafka errors: */
  ERR_UNKNOWN = -1,
  ERR_NO_ERROR = 0,
  ERR_OFFSET_OUT_OF_RANGE = 1,
  ERR_INVALID_MSG = 2,
  ERR_UNKNOWN_TOPIC_OR_PART = 3,
  ERR_INVALID_MSG_SIZE = 4,
  ERR_LEADER_NOT_AVAILABLE = 5,
  ERR_NOT_LEADER_FOR_PARTITION = 6,
  ERR_REQUEST_TIMED_OUT = 7,
  ERR_BROKER_NOT_AVAILABLE = 8,
  ERR_REPLICA_NOT_AVAILABLE = 9,
  ERR_MSG_SIZE_TOO_LARGE = 10,
  ERR_STALE_CTRL_EPOCH = 11,
  ERR_OFFSET_METADATA_TOO_LARGE = 12,
  ERR_OFFSET_LOAD_IN_PROGRESS = 14,
  ERR_CONSUMER_COORDINATOR_NOT_AVAILABLE = 15,
  ERR_NOT_COORDINATOR_FOR_CONSUMER = 16,
  ERR_TOPIC_EXCEPTION = 17,
  ERR_RECORD_LIST_TOO_LARGE = 18,
  ERR_NOT_ENOUGH_REPLICAS = 19,
  ERR_NOT_ENOUGH_REPLICAS_AFTER_APPEND = 20,
  ERR_INVALID_REQUIRED_ACKS = 21,
  ERR_ILLEGAL_GENERATION = 22,
  ERR_INCONSISTENT_GROUP_PROTOCOL = 23,
  ERR_INVALID_GROUP_ID = 24,
  ERR_UNKNOWN_MEMBER_ID = 25,
  ERR_INVALID_SESSION_TIMEOUT = 26,
  ERR_REBALANCE_IN_PROGRESS = 27,
  ERR_INVALID_COMMIT_OFFSET_SIZE = 28,
  ERR_TOPIC_AUTHORIZATION_FAILED = 29,
  ERR_GROUP_AUTHORIZATION_FAILED = 30,
  ERR_CLUSTER_AUTHORIZATION_FAILED = 31

};


/**
 * Returns a human readable representation of a kafka error.
 */
RD_EXPORT
std::string  err2str(RdKafka::ErrorCode err);

/**
 * Wait for all rd_kafka_t objects to be destroyed.
 * Returns 0 if all kafka objects are now destroyed, or -1 if the
 * timeout was reached.
 * Since RdKafka handle deletion is an asynch opration the 
 * `wait_destroyed()` function can be used for applications where
 * a clean shutdown is required.
 */
RD_EXPORT
int          wait_destroyed(int timeout_ms);


/* Forward declarations */
class Producer;
class Message;
class Event;
class Topic;
class TopicPartition;
class Metadata;


/**
 * Delivery Report callback class
 */
class RD_EXPORT DeliveryReportCb {
 public:
  virtual void dr_cb (Message &message) = 0;
};


/**
 * Partitioner callback class
 */
class RD_EXPORT PartitionerCb {
 public:
  virtual int32_t partitioner_cb (const Topic *topic,
                                  const std::string *key,
                                  int32_t partition_cnt,
                                  void *msg_opaque) = 0;
};

/**
 * Variant partitioner callback that gets the key as pointer and length
 * instead of as a const std::string *.
 */
class PartitionerKeyPointerCb {
 public:
  virtual int32_t partitioner_cb (const Topic *topic,
                                  const void *key,
                                  size_t key_len,
                                  int32_t partition_cnt,
                                  void *msg_opaque) = 0;
};


/**
 * SocketCb callback class
 */
class RD_EXPORT SocketCb {
 public:
  virtual int socket_cb (int domain, int type, int protocol) = 0;
};

/**
 * OpenCb callback class
 */
class RD_EXPORT OpenCb {
 public:
  virtual int open_cb (const std::string &path, int flags, int mode) = 0;
};


/**
 * Event callback class
 * Events propogate errors, stats and logs to the application.
 */
class RD_EXPORT EventCb {
 public:
  virtual void event_cb (Event &event) = 0;
};


/**
 * Consume callback class
 */
class RD_EXPORT ConsumeCb {
 public:
  virtual void consume_cb (Message &message, void *opaque) = 0;
};

/**
 * Rebalance callback class
 */
class RD_EXPORT RebalanceCb {
public:
  virtual void rebalance_cb(RdKafka::ErrorCode err,
                            const std::vector<TopicPartition*>&partitions) = 0;
};



/**
 * Event class as provided to the EventCb callback.
 */
class RD_EXPORT Event {
 public:
  enum Type {
    EVENT_ERROR,
    EVENT_STATS,
    EVENT_LOG,
    EVENT_THROTTLE
  };

  enum Severity {
    /* These match the syslog(3) severities */
    EVENT_SEVERITY_EMERG = 0,
    EVENT_SEVERITY_ALERT = 1,
    EVENT_SEVERITY_CRITICAL = 2,
    EVENT_SEVERITY_ERROR = 3,
    EVENT_SEVERITY_WARNING = 4,
    EVENT_SEVERITY_NOTICE = 5,
    EVENT_SEVERITY_INFO = 6,
    EVENT_SEVERITY_DEBUG = 7
  };

  ~Event () {};

  /* Accessor functions */
  virtual Type        type () const = 0;          /* ALL */
  virtual ErrorCode   err () const = 0;           /* ALL but THROTTLE */
  virtual Severity    severity () const = 0;      /* LOG */
  virtual std::string fac () const = 0;           /* LOG */
  virtual std::string str () const = 0;           /* LOG */
  virtual int         throttle_time () const = 0; /* THROTTLE */
  virtual std::string broker_name () const = 0;   /* THROTTLE */
  virtual int         broker_id () const = 0;   /* THROTTLE */
  
};


/**
 * Configuration object.
 * Holds either global or topic configuration.
 * Created through Conf::create() factory.
 */
class RD_EXPORT Conf {
 public:
  enum ConfType {
    CONF_GLOBAL,
    CONF_TOPIC
  };

  enum ConfResult {
    CONF_UNKNOWN = -2,
    CONF_INVALID = -1,
    CONF_OK = 0
  };

  
  /**
   * Create conf object
   */
  static Conf *create (ConfType type);

  virtual ~Conf () { };

  /**
   * Set configuration property 'name' to value 'value'.
   * Returns CONF_OK on success.
   * 'errstr' is set to a human readable error description on error.
   */
  virtual Conf::ConfResult set (const std::string &name,
                                const std::string &value,
                                std::string &errstr) = 0;

  /* Use with 'name' = "dr_cb" */
  virtual Conf::ConfResult set (const std::string &name,
                                DeliveryReportCb *dr_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "event_cb" */
  virtual Conf::ConfResult set (const std::string &name,
                                EventCb *event_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "default_topic_conf" */
  virtual Conf::ConfResult set (const std::string &name,
                                const Conf *topic_conf,
                                std::string &errstr) = 0;

  /* Use with 'name' = "partitioner_cb" */
  virtual Conf::ConfResult set (const std::string &name,
                                PartitionerCb *partitioner_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "partitioner_key_pointer_cb" */
  virtual Conf::ConfResult set (const std::string &name,
                                PartitionerKeyPointerCb *partitioner_kp_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "socket_cb" */
  virtual Conf::ConfResult set (const std::string &name, SocketCb *socket_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "open_cb" */
  virtual Conf::ConfResult set (const std::string &name, OpenCb *open_cb,
                                std::string &errstr) = 0;

  /* Use with 'name' = "rebalance_cb" */
  virtual Conf::ConfResult set (const std::string &name,
                                RebalanceCb *rebalance_cb,
                                std::string &errstr) = 0;

  /* Query single configuration value */
  virtual Conf::ConfResult get(const std::string &name,
	  std::string &value) const = 0;

  /**
   * Dump configuration names and values to list containing name,value tuples
   */
  virtual std::list<std::string> *dump () = 0;
};


/**
 * Base handle, super class for Consumer and Producer.
 */
class RD_EXPORT Handle {
 public:
  virtual ~Handle() {};

  /**
   * Returns the name of the handle
   */
  virtual const std::string name () const = 0;


  /**
   * Polls the provided kafka handle for events.
   *
   * Events will cause application provided callbacks to be called.
   *
   * The 'timeout_ms' argument specifies the minimum amount of time
   * (in milliseconds) that the call will block waiting for events.
   * For non-blocking calls, provide 0 as 'timeout_ms'.
   * To wait indefinately for an event, provide -1.
   *
   * Events:
   *   - delivery report callbacks  (if dr_cb is configured) [producer]
   *   - event callbacks (if event_cb is configured) [producer & consumer]
   *   - stats callbacks (if stats_cb is configured) [producer & consumer]
   *
   * Returns the number of events served.
   */
  virtual int poll (int timeout_ms) = 0;

  /**
   * Returns the current out queue length:
   * messages waiting to be sent to, or acknowledged by, the broker.
   */
  virtual int outq_len () = 0;

  /**
   * Request Metadata from broker.
   *  all_topics - if non-zero: request info about all topics in cluster,
   *               if zero: only request info about locally known topics.
   *  only_rkt   - only request info about this topic
   *  metadatap  - pointer to hold metadata result.
   *               The '*metadatap' pointer must be released
   *               with delete.
   *  timeout_ms - maximum response time before failing.
   *
   * Returns RD_KAFKA_RESP_ERR_NO_ERROR on success (in which case *metadatap)
   * will be set, else RD_KAFKA_RESP_ERR__TIMED_OUT on timeout or
   * other error code on error.
   */
  virtual ErrorCode metadata (bool all_topics, const Topic *only_rkt,
                              Metadata **metadatap, int timeout_ms) = 0;
};


/**
 * Topic and Partition
 */
class RD_EXPORT TopicPartition {
public:
  static TopicPartition *create (const std::string &topic, int partition);

  virtual ~TopicPartition() = 0;

  virtual int partition () = 0;
  virtual const std::string &topic () const = 0;
};


/**
 * Topic
 */
class RD_EXPORT Topic {
 public:
  /**
   * Unassigned partition.
   *
   * The unassigned partition is used by the producer API for messages
   * that should be partitioned using the configured or default partitioner.
   */
  static const int32_t PARTITION_UA = -1;

  /* Special offsets */
  static const int64_t OFFSET_BEGINNING = -2; /* Consume from beginning */
  static const int64_t OFFSET_END       = -1; /* Consume from end */
  static const int64_t OFFSET_STORED    = -1000; /* Consume from offset
                                                  * retrived from offset store*/

  /**
   * Creates a new topic handle for topic named 'topic_str'.
   *
   * 'conf' is an optional configuration for the topic  that will be used 
   * instead of the default topic configuration.
   * The 'conf' object is reusable after this call.
   *
   * Returns the new topic handle or NULL on error (see `errstr`).
   */
  static Topic *create (Handle *base, const std::string &topic_str,
                        Conf *conf, std::string &errstr);

  virtual ~Topic () = 0;

  /**
   * Returns the topic name
   */
  virtual const std::string name () const = 0;

  /**
   * Returns true if 'partition' is available for the topic (has leader).
   * NOTE: MUST ONLY be called from within a PartitionerCb callback.
   */
  virtual bool partition_available (int32_t partition) const = 0;

  /**
   * Store offset 'offset' for topic partition 'partition'.
   * The offset will be commited (written) to the offset store according
   * to `auto.commit.interval.ms`.
   *
   * NOTE: `auto.commit.enable` must be set to "false" when using this API.
   *
   * Returns RD_KAFKA_RESP_ERR_NO_ERROR on success or an error code on error.
   */
  virtual ErrorCode offset_store (int32_t partition, int64_t offset) = 0;
};


/**
 * Message, as provided to DeliveryReportCb, PartitionerCb callbacks, etc.
 */
class RD_EXPORT Message {
 public:
  /* Accessor functions,
   * Not all fields are present in all types of callbacks. */
  virtual std::string         errstr() const = 0;
  virtual ErrorCode           err () const = 0;
  /* NOTE: topic() will return NULL if the corresponding Topic object has not been
   *       explicitly created with Topic::create().
   *       In this case use topic_name() instead. */
  virtual Topic              *topic () const = 0;
  virtual std::string         topic_name () const = 0;
  virtual int32_t             partition () const = 0;
  virtual void               *payload () const = 0 ;
  virtual size_t              len () const = 0;
  virtual const std::string  *key () const = 0;
  virtual const void         *key_pointer () const = 0 ;
  virtual size_t              key_len () const = 0;
  virtual int64_t             offset () const = 0;
  virtual void               *msg_opaque () const = 0;
  virtual ~Message () = 0;
};


/**
 * Queue
 *
 * Create a new message queue.  Message queues allows the application
 * to re-route consumed messages from multiple topic+partitions into
 * one single queue point.  This queue point, containing messages from
 * a number of topic+partitions, may then be served by a single
 * consume() method, rather than one per topic+partition combination.
 *
 * See the Consumer::start(), Consumer::consume(), and
 * Consumer::consume_callback() methods that take a queue as the first
 * parameter for more information.
 */
class Queue {
 public:

  /**
   * Create Queue object
   */
  static Queue *create (Handle *handle);

  virtual ~Queue () { }
};

/**
 * High-level KafkaConsumer (for brokers 0.9 and later)
 */
class RD_EXPORT KafkaConsumer : public virtual Handle {
public:
  static KafkaConsumer *create (Conf *conf, std::string &errstr);

  virtual ~KafkaConsumer () = 0;


  virtual ErrorCode assignment (std::vector<RdKafka::TopicPartition*> &partitions) = 0;
  virtual ErrorCode subscription (std::vector<std::string> &topics) = 0;


  /* pattern matching automatically performed for topics prefixed with "^" */
  virtual ErrorCode subscribe (const std::vector<std::string> &topics) = 0;

  virtual ErrorCode unsubscribe () = 0;

  virtual ErrorCode assign (const std::vector<TopicPartition*> &partitions) = 0;

  virtual Message *consume (int timeout_ms) = 0;

#if FIXME
  virtual ErrorCode commitSync () = 0;
  // FIXME virtual ErrorCode commitSync (Map<TopicPartition, Offset> offsets) = 0;
  // FIXME virtual ErrorCode commitAsync (OffsetCommitCallback *callback) = 0;

  virtual ErrorCode seek (TopicPartition &partition, int64_t offset) = 0;
  virtual ErrorCode seek (Topic *topic, int64_t offset) = 0,
  virtual ErrorCode seekToBeginning (TopicPartition &partition) = 0;
  virtual ErrorCode seekToEnd (TopicPartition &partition) = 0;

  virtual ErrorCode position (TopicPartition &partition, int64_t *offsetp) = 0;

  // FIXME virtual ErrorCode committed (TopicPartition &partition, OffsetAndMetadata *oam) = 0;
#endif

  virtual ErrorCode close () = 0;
};

/**
 * Consumer
 */
class RD_EXPORT Consumer : public virtual Handle {
 public:
  /**
   * Creates a new Kafka consumer handle.
   *
   * 'conf' is an optional object that will be used instead of the default
   * configuration.
   * The 'conf' object is reusable after this call.
   *
   * Returns the new handle on success or NULL on error in which case
   * 'errstr' is set to a human readable error message.
   */
  static Consumer *create (Conf *conf, std::string &errstr);

  virtual ~Consumer () = 0;


  /**
   * Start consuming messages for topic and 'partition'
   * at offset 'offset' which may either be a proper offset (0..N)
   * or one of the the special offsets:
   *  `OFFSET_BEGINNING` or `OFFSET_END`.
   *
   * rdkafka will attempt to keep 'queued.min.messages' (config property)
   * messages in the local queue by repeatedly fetching batches of messages
   * from the broker until the threshold is reached.
   *
   * The application shall use one of the `..->consume*()` functions
   * to consume messages from the local queue, each kafka message being
   * represented as a `RdKafka::Message *` object.
   *
   * `..->start()` must not be called multiple times for the same
   * topic and partition without stopping consumption first with
   * `..->stop()`.
   *
   * Returns an ErrorCode to indicate success or failure.
   */
  virtual ErrorCode start (Topic *topic, int32_t partition, int64_t offset) = 0;

  /**
   * Start consuming messages for topic and 'partition' on queue 'queue'.
   *
   * See start() above for additional details on parameters and results.
   */
  virtual ErrorCode start (Topic *topic, int32_t partition, int64_t offset,
                           Queue *queue) = 0;

  /**
   * Stop consuming messages for topic and 'partition', purging
   * all messages currently in the local queue.
   *
   * The application needs to be stop all consumers before destroying
   * the Consumer handle.
   *
   * Returns 0 on success or -1 on error (see `errno`).
   */
  virtual ErrorCode stop (Topic *topic, int32_t partition) = 0;

  /**
   * Consume a single message from topic and 'partition'.
   *
   * 'timeout_ms' is maximum amount of time to wait for a message to be
   * received.
   * Consumer must have been previously started with `..->start()`.
   *
   * Returns a Message object, the application needs to check if message
   * is an error or a proper message `Message->err()` and checking for
   * `ERR_NO_ERROR`.
   *
   * The message object must be destroyed when the application is done with it.
   *
   * Errors (in Message->err()):
   *   ERR__TIMED_OUT - 'timeout_ms' was reached with no new messages fetched.
   *   ERR__PARTITION_EOF - End of partition reached, not an error.
   */
  virtual Message *consume (Topic *topic, int32_t partition,
                            int timeout_ms) = 0;

  /**
   * Consume a single message from the specified queue.
   *
   * 'timeout_ms' is maximum amount of time to wait for a message to be
   * received.
   * Consumer must have been previously started on the queue with
   * `..->start()`.
   *
   * Returns a Message object, the application needs to check if message
   * is an error or a proper message `Message->err()` and checking for
   * `ERR_NO_ERROR`.
   *
   * The message object must be destroyed when the application is done with it.
   *
   * Errors (in Message->err()):
   *   ERR__TIMED_OUT - 'timeout_ms' was reached with no new messages fetched.
   *
   * Note that Message->topic() may be nullptr after certain kinds of
   * errors, so applications should check that it isn't null before
   * dereferencing it.
   */
  virtual Message *consume (Queue *queue, int timeout_ms) = 0;

  /**
   * Consumes messages from 'topic' and 'partition', calling
   * the provided callback for each consumed messsage.
   *
   * `consume_callback()` provides higher throughput performance
   * than `consume()`.
   *
   * 'timeout_ms' is the maximum amount of time to wait for one or more messages
   * to arrive.
   *
   * The provided 'consume_cb' instance has its 'consume_cb' function
   * called for every message received.
   *
   * The 'opaque' argument is passed to the 'consume_cb' as 'opaque'.
   *
   * Returns the number of messages processed or -1 on error.
   *
   * See: consume()
   */
  virtual int consume_callback (Topic *topic, int32_t partition,
                                int timeout_ms,
                                ConsumeCb *consume_cb,
                                void *opaque) = 0;

  /**
   * Consumes messages from 'queue', calling the provided callback for
   * each consumed messsage.
   *
   * `consume_callback()` provides higher throughput performance
   * than `consume()`.
   *
   * 'timeout_ms' is the maximum amount of time to wait for one or more messages
   * to arrive.
   *
   * The provided 'consume_cb' instance has its 'consume_cb' function
   * called for every message received.
   *
   * The 'opaque' argument is passed to the 'consume_cb' as 'opaque'.
   *
   * Returns the number of messages processed or -1 on error.
   *
   * See: consume()
   */
  virtual int consume_callback (Queue *queue, int timeout_ms,
                                RdKafka::ConsumeCb *consume_cb,
                                void *opaque) = 0;

  /**
   * Converts an offset into the logical offset from the tail of a topic.
   *
   * 'offset' is the (positive) number of items from the end.
   */
  static int64_t OffsetTail(int64_t offset);
};


/**
 * Producer
 */
class RD_EXPORT Producer : public virtual Handle {
 public:
  /**
   * Creates a new Kafka producer handle.
   *
   * 'conf' is an optional object that will be used instead of the default
   * configuration.
   * The 'conf' object is reusable after this call.
   *
   * Returns the new handle on success or NULL on error in which case
   * 'errstr' is set to a human readable error message.
   */
  static Producer *create (Conf *conf, std::string &errstr);


  virtual ~Producer () = 0;

  /* Produce msgflags */
  static const int RK_MSG_FREE = 0x1;
  static const int RK_MSG_COPY = 0x2;
  /* For backwards compatibility: */
#ifndef MSG_COPY /* defined in sys/msg.h */
  static const int MSG_FREE = RK_MSG_FREE;
  static const int MSG_COPY = RK_MSG_COPY;
#endif

  /**
   * Produce and send a single message to broker.
   *
   * This is an asynch non-blocking API.
   *
   * 'partition' is the target partition, either:
   *   - RdKafka::Topic::PARTITION_UA (unassigned) for
   *     automatic partitioning using the topic's partitioner function, or
   *   - a fixed partition (0..N)
   *
   * 'msgflags' is zero or more of the following flags OR:ed together:
   *    RK_MSG_FREE - rdkafka will free(3) 'payload' when it is done with it.
   *    RK_MSG_COPY - the 'payload' data will be copied and the 'payload'
   *               pointer will not be used by rdkafka after the
   *               call returns.
   *
   *  NOTE: RK_MSG_FREE and RK_MSG_COPY are mutually exclusive.
   *
   *  If the function returns -1 and RK_MSG_FREE was specified, then
   *  the memory associated with the payload is still the caller's
   *  responsibility.
   *
   * 'payload' is the message payload of size 'len' bytes.
   *
   * 'key' is an optional message key, if non-NULL it
   * will be passed to the topic partitioner as well as be sent with the
   * message to the broker and passed on to the consumer.
   *
   * 'msg_opaque' is an optional application-provided per-message opaque
   * pointer that will provided in the delivery report callback (`dr_cb`) for
   * referencing this message.
   *
   * Returns an ErrorCode to indicate success or failure.
   *  ERR__QUEUE_FULL - maximum number of outstanding messages has been reached:
   *                   "queue.buffering.max.message"
   *
   *  ERR_MSG_SIZE_TOO_LARGE - message is larger than configured max size:
   *                           "messages.max.bytes".
   *
   *  ERR__UNKNOWN_PARTITION - requested 'partition' is unknown in the
   *                           Kafka cluster.
   *
   *  ERR__UNKNOWN_TOPIC     - topic is unknown in the Kafka cluster.
   */
  virtual ErrorCode produce (Topic *topic, int32_t partition,
                             int msgflags,
                             void *payload, size_t len,
                             const std::string *key,
                             void *msg_opaque) = 0;

  /**
   * Variant produce() that passes the key as a pointer and length
   * instead of as a const std::string *.
   */
  virtual ErrorCode produce (Topic *topic, int32_t partition,
                             int msgflags,
                             void *payload, size_t len,
                             const void *key, size_t key_len,
                             void *msg_opaque) = 0;
};



/**
 * Metadata: Broker information
 */
class BrokerMetadata {
 public:
  /**
   * Broker id
   */
  virtual int32_t id() const = 0;

  /**
   * Broker hostname
   */
  virtual const std::string host() const = 0;

  /**
   * Broker listening port
   */
  virtual int port() const = 0;

  virtual ~BrokerMetadata() = 0;
};



/**
 * Metadata: Partition information
 */
class PartitionMetadata {
 public:
  typedef std::vector<int32_t> ReplicasVector;
  typedef std::vector<int32_t> ISRSVector;

  typedef ReplicasVector::const_iterator ReplicasIterator;
  typedef ISRSVector::const_iterator     ISRSIterator;


  /**
   * Partition id
   */
  virtual int32_t id() const = 0;

  /**
   * Partition error reported by broker
   */
  virtual ErrorCode err() const = 0;

  /**
   * Leader broker (id) for partition
   */
  virtual int32_t leader() const = 0;

  /**
   * Replica brokers
   */
  virtual const std::vector<int32_t> *replicas() const = 0;

  /**
   * In-Sync-Replica brokers
   * NOTE: The broker may return a cached/outdated list of ISRs.
   */
  virtual const std::vector<int32_t> *isrs() const = 0;

  virtual ~PartitionMetadata() = 0;
};



/**
 * Metadata: Topic information
 */
class TopicMetadata {
 public:
  typedef std::vector<const PartitionMetadata*> PartitionMetadataVector;
  typedef PartitionMetadataVector::const_iterator PartitionMetadataIterator;

  /**
   * Topic name
   */
  virtual const std::string topic() const = 0;

  /**
   * Partition list
   */
  virtual const PartitionMetadataVector *partitions() const = 0;

  /**
   * Topic error reported by broker
   */
  virtual ErrorCode err() const = 0;

  virtual ~TopicMetadata() = 0;
};


/**
 * Metadata container
 */
class Metadata {
 public:
  typedef std::vector<const BrokerMetadata*> BrokerMetadataVector;
  typedef std::vector<const TopicMetadata*>  TopicMetadataVector;

  typedef BrokerMetadataVector::const_iterator BrokerMetadataIterator;
  typedef TopicMetadataVector::const_iterator  TopicMetadataIterator;


  /**
   * Broker list
   */
  virtual const BrokerMetadataVector *brokers() const = 0;

  /**
   * Topic list
   */
  virtual const TopicMetadataVector  *topics() const = 0;

  /**
   * Broker (id) originating this metadata
   */
  virtual int32_t orig_broker_id() const = 0;

  /**
   * Broker (name) originating this metadataa
   */
  virtual const std::string orig_broker_name() const = 0;

  virtual ~Metadata() = 0;
};


}

