/*
 *
 * Author: Thomas Pasquier <tfjmp@seas.harvard.edu>
 *
 * Copyright (C) 2016 Harvard University
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 *
 */
#ifndef _LINUX_PROVENANCE_FILTER_H
#define _LINUX_PROVENANCE_FILTER_H

#include <uapi/linux/provenance.h>

#include "provenance_policy.h"
#include "provenance_ns.h"
#include "provenance_secctx.h"

#define HIT_FILTER(filter, data) ((filter & data) != 0)

extern uint64_t prov_node_filter;
extern uint64_t prov_propagate_node_filter;

#define filter_node(node) __filter_node(prov_node_filter, node)
#define filter_propagate_node(node) __filter_node(prov_propagate_node_filter, node)

/* return either or not the node should be filtered out */
static inline bool __filter_node(uint64_t filter, prov_entry_t *node)
{
	if (!prov_policy.prov_enabled)
		return true;
	if (provenance_is_opaque(node))
		return true;
	// we hit an element of the black list ignore
	if (HIT_FILTER(filter, node_identifier(node).type))
		return true;
	return false;
}

#define UPDATE_FILTER (SUBTYPE(RL_VERSION_PROCESS) | SUBTYPE(RL_VERSION) | SUBTYPE(RL_NAMED))
static inline bool filter_update_node(uint64_t relation_type)
{
	if (HIT_FILTER(UPDATE_FILTER, relation_type)) // not update if relation is of above type
		return true;
	return false;
}

extern uint64_t prov_relation_filter;
extern uint64_t prov_propagate_relation_filter;

/* return either or not the relation should be filtered out */
static inline bool filter_relation(uint64_t type)
{
	// we hit an element of the black list ignore
	if (HIT_FILTER(prov_relation_filter, type))
		return true;
	return false;
}

/* return either or not tracking should propagate */
static inline bool filter_propagate_relation(uint64_t type)
{
	// the relation does not allow tracking propagation
	if (HIT_FILTER(prov_propagate_relation_filter, type))
		return true;
	return false;
}

static inline bool should_record_relation(uint64_t type, union prov_elt *from, union prov_elt *to)
{
	if (filter_relation(type))
		return false;
	// one of the node should not appear in the record, ignore the relation
	if (filter_node((prov_entry_t *)from) || filter_node((prov_entry_t *)to))
		return false;
	return true;
}

static inline bool prov_has_secid(union prov_elt *prov)
{
	switch (prov_type(prov)) {
	case ENT_INODE_UNKNOWN:
	case ENT_INODE_LINK:
	case ENT_INODE_FILE:
	case ENT_INODE_DIRECTORY:
	case ENT_INODE_CHAR:
	case ENT_INODE_BLOCK:
	case ENT_INODE_FIFO:
	case ENT_INODE_SOCKET:
	case ENT_INODE_MMAP:
		return true;
	default: return false;
	}
}

static inline void apply_target(union prov_elt *prov)
{
	uint8_t op;

	// track based on ns
	if (prov_type(prov) == ACT_TASK) {
		op = prov_ns_whichOP(prov->task_info.utsns,
										prov->task_info.ipcns,
										prov->task_info.mntns,
										prov->task_info.pidns,
										prov->task_info.netns,
										prov->task_info.cgroupns);
		if (unlikely(op != 0)) {
			pr_info("Provenance: apply ns filter %u.", op);
			if ((op & PROV_NS_TRACKED) != 0)
				set_tracked(prov);
			if ((op & PROV_NS_PROPAGATE) != 0)
				set_propagate(prov);
			if ((op & PROV_NS_OPAQUE) != 0)
				set_opaque(prov);
		}
	}
	if (prov_has_secid(prov)) {
		op = prov_secctx_whichOP(node_secid(prov));
		if (unlikely(op != 0)) {
			pr_info("Provenance: apply secctx filter %u.", op);
			if ((op & PROV_SEC_TRACKED) != 0)
				set_tracked(prov);
			if ((op & PROV_SEC_PROPAGATE) != 0)
				set_propagate(prov);
			if ((op & PROV_SEC_OPAQUE) != 0)
				set_opaque(prov);
		}
	}
}
#endif
