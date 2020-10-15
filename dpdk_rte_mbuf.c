#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <limits.h>

#define __rte_aligned(a) __attribute__((__aligned__(a)))
/** Minimum Cache line size. */
#define RTE_CACHE_LINE_MIN_SIZE 64

/** Force alignment to cache line. */
#define __rte_cache_aligned __rte_aligned(RTE_CACHE_LINE_MIN_SIZE)

/** Force minimum cache line alignment. */
#define __rte_cache_min_aligned __rte_aligned(RTE_CACHE_LINE_MIN_SIZE)

/** Generic marker for any place in a structure. */
__extension__ typedef void *RTE_MARKER[0];
/** Marker for 1B alignment in a structure. */
__extension__ typedef uint8_t RTE_MARKER8[0];
/** Marker for 2B alignment in a structure. */
__extension__ typedef uint16_t RTE_MARKER16[0];
/** Marker for 4B alignment in a structure. */
__extension__ typedef uint32_t RTE_MARKER32[0];
/** Marker for 8B alignment in a structure. */
__extension__ typedef uint64_t RTE_MARKER64[0];

#define RTE_STD_C11 __extension__

struct rte_mbuf_sched {
	uint32_t queue_id; /**< Queue ID. */
	uint8_t traffic_class;
	/**< Traffic class ID. Traffic class 0
	 * is the highest priority traffic class.
	 */
	uint8_t color;
	/**< Color. @see enum rte_color.*/
	uint16_t reserved; /**< Reserved. */
};			   /**< Hierarchical scheduler */

enum { RTE_MBUF_L2_LEN_BITS    = 7,
       RTE_MBUF_L3_LEN_BITS    = 9,
       RTE_MBUF_L4_LEN_BITS    = 8,
       RTE_MBUF_TSO_SEGSZ_BITS = 16,
       RTE_MBUF_OUTL3_LEN_BITS = 9,
       RTE_MBUF_OUTL2_LEN_BITS = 7,
       RTE_MBUF_TXOFLD_UNUSED_BITS =
	       sizeof(uint64_t) * CHAR_BIT - RTE_MBUF_L2_LEN_BITS -
	       RTE_MBUF_L3_LEN_BITS - RTE_MBUF_L4_LEN_BITS -
	       RTE_MBUF_TSO_SEGSZ_BITS - RTE_MBUF_OUTL3_LEN_BITS -
	       RTE_MBUF_OUTL2_LEN_BITS,
#if RTE_BYTE_ORDER == RTE_BIG_ENDIAN
       RTE_MBUF_L2_LEN_OFS = sizeof(uint64_t) * CHAR_BIT - RTE_MBUF_L2_LEN_BITS,
       RTE_MBUF_L3_LEN_OFS = RTE_MBUF_L2_LEN_OFS - RTE_MBUF_L3_LEN_BITS,
       RTE_MBUF_L4_LEN_OFS = RTE_MBUF_L3_LEN_OFS - RTE_MBUF_L4_LEN_BITS,
       RTE_MBUF_TSO_SEGSZ_OFS = RTE_MBUF_L4_LEN_OFS - RTE_MBUF_TSO_SEGSZ_BITS,
       RTE_MBUF_OUTL3_LEN_OFS =
	       RTE_MBUF_TSO_SEGSZ_OFS - RTE_MBUF_OUTL3_LEN_BITS,
       RTE_MBUF_OUTL2_LEN_OFS =
	       RTE_MBUF_OUTL3_LEN_OFS - RTE_MBUF_OUTL2_LEN_BITS,
       RTE_MBUF_TXOFLD_UNUSED_OFS =
	       RTE_MBUF_OUTL2_LEN_OFS - RTE_MBUF_TXOFLD_UNUSED_BITS,
#else
       RTE_MBUF_L2_LEN_OFS    = 0,
       RTE_MBUF_L3_LEN_OFS    = RTE_MBUF_L2_LEN_OFS + RTE_MBUF_L2_LEN_BITS,
       RTE_MBUF_L4_LEN_OFS    = RTE_MBUF_L3_LEN_OFS + RTE_MBUF_L3_LEN_BITS,
       RTE_MBUF_TSO_SEGSZ_OFS = RTE_MBUF_L4_LEN_OFS + RTE_MBUF_L4_LEN_BITS,
       RTE_MBUF_OUTL3_LEN_OFS =
	       RTE_MBUF_TSO_SEGSZ_OFS + RTE_MBUF_TSO_SEGSZ_BITS,
       RTE_MBUF_OUTL2_LEN_OFS =
	       RTE_MBUF_OUTL3_LEN_OFS + RTE_MBUF_OUTL3_LEN_BITS,
       RTE_MBUF_TXOFLD_UNUSED_OFS =
	       RTE_MBUF_OUTL2_LEN_OFS + RTE_MBUF_OUTL2_LEN_BITS,
#endif
};

typedef uint64_t rte_iova_t;
typedef struct {
	volatile int16_t cnt; /**< An internal counter value. */
} rte_atomic16_t;
/**
 * The generic rte_mbuf, containing a packet mbuf.
 */
struct rte_mbuf {
	RTE_MARKER cacheline0;

	void *buf_addr; /**< Virtual address of segment buffer. */
	/**
	 * Physical address of segment buffer.
	 * Force alignment to 8-bytes, so as to ensure we have the exact
	 * same mbuf cacheline0 layout for 32-bit and 64-bit. This makes
	 * working on vector drivers easier.
	 */
	RTE_STD_C11
	union {
		rte_iova_t buf_iova;
		rte_iova_t buf_physaddr; /**< deprecated */
	} __rte_aligned(sizeof(rte_iova_t));

	/* next 8 bytes are initialised on RX descriptor rearm */
	RTE_MARKER64 rearm_data;
	uint16_t data_off;

	/**
	 * Reference counter. Its size should at least equal to the size
	 * of port field (16 bits), to support zero-copy broadcast.
	 * It should only be accessed using the following functions:
	 * rte_mbuf_refcnt_update(), rte_mbuf_refcnt_read(), and
	 * rte_mbuf_refcnt_set(). The functionality of these functions (atomic,
	 * or non-atomic) is controlled by the CONFIG_RTE_MBUF_REFCNT_ATOMIC
	 * config option.
	 */
	RTE_STD_C11
	union {
		rte_atomic16_t refcnt_atomic; /**< Atomically accessed refcnt */
		/** Non-atomically accessed refcnt */
		uint16_t refcnt;
	};
	uint16_t nb_segs; /**< Number of segments. */

	/** Input port (16 bits to support more than 256 virtual ports).
	 * The event eth Tx adapter uses this field to specify the output port.
	 */
	uint16_t port;

	uint64_t ol_flags; /**< Offload features. */

	/* remaining bytes are set on RX when pulling packet from descriptor */
	RTE_MARKER rx_descriptor_fields1;

	/*
	 * The packet type, which is the combination of outer/inner L2, L3, L4
	 * and tunnel types. The packet_type is about data really present in the
	 * mbuf. Example: if vlan stripping is enabled, a received vlan packet
	 * would have RTE_PTYPE_L2_ETHER and not RTE_PTYPE_L2_VLAN because the
	 * vlan is stripped from the data.
	 */
	RTE_STD_C11
	union {
		uint32_t packet_type; /**< L2/L3/L4 and tunnel information. */
		struct {
			uint32_t l2_type : 4;  /**< (Outer) L2 type. */
			uint32_t l3_type : 4;  /**< (Outer) L3 type. */
			uint32_t l4_type : 4;  /**< (Outer) L4 type. */
			uint32_t tun_type : 4; /**< Tunnel type. */
			RTE_STD_C11
			union {
				uint8_t inner_esp_next_proto;
				/**< ESP next protocol type, valid if
				 * RTE_PTYPE_TUNNEL_ESP tunnel type is set
				 * on both Tx and Rx.
				 */
				__extension__ struct {
					uint8_t inner_l2_type : 4;
					/**< Inner L2 type. */
					uint8_t inner_l3_type : 4;
					/**< Inner L3 type. */
				};
			};
			uint32_t inner_l4_type : 4; /**< Inner L4 type. */
		};
	};

	uint32_t pkt_len;  /**< Total pkt len: sum of all segments. */
	uint16_t data_len; /**< Amount of data in segment buffer. */
	/** VLAN TCI (CPU order), valid if PKT_RX_VLAN is set. */
	uint16_t vlan_tci;

	RTE_STD_C11
	union {
		union {
			uint32_t rss; /**< RSS hash result if RSS enabled */
			struct {
				union {
					struct {
						uint16_t hash;
						uint16_t id;
					};
					uint32_t lo;
					/**< Second 4 flexible bytes */
				};
				uint32_t hi;
				/**< First 4 flexible bytes or FD ID, dependent
				 * on PKT_RX_FDIR_* flag in ol_flags.
				 */
			} fdir; /**< Filter identifier if FDIR enabled */
			struct rte_mbuf_sched sched;
			/**< Hierarchical scheduler : 8 bytes */
			struct {
				uint32_t reserved1;
				uint16_t reserved2;
				uint16_t txq;
				/**< The event eth Tx adapter uses this field
				 * to store Tx queue id.
				 * @see rte_event_eth_tx_adapter_txq_set()
				 */
			} txadapter; /**< Eventdev ethdev Tx adapter */
			/**< User defined tags. See rte_distributor_process() */
			uint32_t usr;
		} hash; /**< hash information */
	};

	/** Outer VLAN TCI (CPU order), valid if PKT_RX_QINQ is set. */
	uint16_t vlan_tci_outer;

	uint16_t buf_len; /**< Length of segment buffer. */

	/** Valid if PKT_RX_TIMESTAMP is set. The unit and time reference
	 * are not normalized but are always the same for a given port.
	 * Some devices allow to query rte_eth_read_clock that will return the
	 * current device timestamp.
	 */
	uint64_t timestamp;

	/* second cache line - fields only used in slow path or on TX */
	RTE_MARKER cacheline1 __rte_cache_min_aligned;

	RTE_STD_C11
	union {
		void *userdata;	  /**< Can be used for external metadata */
		uint64_t udata64; /**< Allow 8-byte userdata on 32-bit */
	};

	void *pool;	       /**< Pool from which mbuf was allocated. */
	struct rte_mbuf *next; /**< Next segment of scattered packet. */

	/* fields to support TX offloads */
	RTE_STD_C11
	union {
		uint64_t tx_offload; /**< combined for easy fetch */
		__extension__ struct {
			uint64_t l2_len : RTE_MBUF_L2_LEN_BITS;
			/**< L2 (MAC) Header Length for non-tunneling pkt.
			 * Outer_L4_len + ... + Inner_L2_len for tunneling pkt.
			 */
			uint64_t l3_len : RTE_MBUF_L3_LEN_BITS;
			/**< L3 (IP) Header Length. */
			uint64_t l4_len : RTE_MBUF_L4_LEN_BITS;
			/**< L4 (TCP/UDP) Header Length. */
			uint64_t tso_segsz : RTE_MBUF_TSO_SEGSZ_BITS;
			/**< TCP TSO segment size */

			/*
			 * Fields for Tx offloading of tunnels.
			 * These are undefined for packets which don't request
			 * any tunnel offloads (outer IP or UDP checksum,
			 * tunnel TSO).
			 *
			 * PMDs should not use these fields unconditionally
			 * when calculating offsets.
			 *
			 * Applications are expected to set appropriate tunnel
			 * offload flags when they fill in these fields.
			 */
			uint64_t outer_l3_len : RTE_MBUF_OUTL3_LEN_BITS;
			/**< Outer L3 (IP) Hdr Length. */
			uint64_t outer_l2_len : RTE_MBUF_OUTL2_LEN_BITS;
			/**< Outer L2 (MAC) Hdr Length. */

			/* uint64_t unused:RTE_MBUF_TXOFLD_UNUSED_BITS; */
		};
	};

	/** Size of the application private data. In case of an indirect
	 * mbuf, it stores the direct mbuf private data size.
	 */
	uint16_t priv_size;

	/** Timesync flags for use with IEEE1588. */
	uint16_t timesync;

	/** Sequence number. See also rte_reorder_insert(). */
	uint32_t seqn;

	/** Shared data for external buffer attached to mbuf. See
	 * rte_pktmbuf_attach_extbuf().
	 */
	void *shinfo;

	uint64_t dynfield1[2]; /**< Reserved for dynamic fields. */
} __rte_cache_aligned;

int main(void)
{
	printf("sizeof dpdk mbuf size is:%"PRIu64"\n", sizeof(struct rte_mbuf));
}
