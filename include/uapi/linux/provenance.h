/*
*
* Author: Thomas Pasquier <tfjmp2@cam.ac.uk>
*
* Copyright (C) 2015 University of Cambridge
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
*/
#ifndef _UAPI_LINUX_PROVENANCE_H
#define _UAPI_LINUX_PROVENANCE_H

#ifndef __KERNEL__
#include <linux/limits.h>
#include <linux/ifc.h>
#else
#include <uapi/linux/ifc.h>
#include <uapi/linux/limits.h>
#endif

#define PROV_ENABLE_FILE                      "/sys/kernel/security/provenance/enable"
#define PROV_ALL_FILE                         "/sys/kernel/security/provenance/all"
#define PROV_OPAQUE_FILE                      "/sys/kernel/security/provenance/opaque"
#define PROV_TRACKED_FILE                     "/sys/kernel/security/provenance/tracked"
#define PROV_PROPAGATE_FILE                   "/sys/kernel/security/provenance/propagate"
#define PROV_NODE_FILE                        "/sys/kernel/security/provenance/node"
#define PROV_RELATION_FILE                    "/sys/kernel/security/provenance/relation"
#define PROV_SELF_FILE                        "/sys/kernel/security/provenance/self"
#define PROV_MACHINE_ID_FILE                  "/sys/kernel/security/provenance/machine_id"
#define PROV_NODE_FILTER_FILE                 "/sys/kernel/security/provenance/node_filter"
#define PROV_RELATION_FILTER_FILE             "/sys/kernel/security/provenance/relation_filter"
#define PROV_PROPAGATE_NODE_FILTER_FILE       "/sys/kernel/security/provenance/propagate_node_filter"
#define PROV_PROPAGATE_RELATION_FILTER_FILE   "/sys/kernel/security/provenance/propagate_relation_filter"
#define PROV_FLUSH_FILE                       "/sys/kernel/security/provenance/flush"
#define PROV_FILE_FILE                        "/sys/kernel/security/provenance/file"

#define PROV_RELAY_NAME                       "/sys/kernel/debug/provenance"
#define PROV_LONG_RELAY_NAME                  "/sys/kernel/debug/long_provenance"

#define MSG_STR               0x00000001UL
#define MSG_RELATION          0x00000002UL
#define MSG_TASK              0x00000004UL
#define MSG_INODE_UNKNOWN     0x00000008UL
#define MSG_INODE_LINK        0x00000010UL
#define MSG_INODE_FILE        0x00000020UL
#define MSG_INODE_DIRECTORY   0x00000040UL
#define MSG_INODE_CHAR        0x00000080UL
#define MSG_INODE_BLOCK       0x00000100UL
#define MSG_INODE_FIFO        0x00000200UL
#define MSG_INODE_SOCKET      0x00000400UL
#define MSG_MSG               0x00000800UL
#define MSG_SHM               0x00001000UL
#define MSG_SOCK              0x00002000UL
#define MSG_ADDR              0x00004000UL
#define MSG_SB                0x00008000UL
#define MSG_FILE_NAME         0x00010000UL
#define MSG_IFC               0x00020000UL
#define MSG_DISC_ENTITY       0x00040000UL
#define MSG_DISC_ACTIVITY     0x00080000UL
#define MSG_DISC_AGENT        0x00100000UL
#define MSG_DISC_NODE         0x00200000UL

#define RL_READ               0x00000001UL
#define RL_WRITE              0x00000002UL
#define RL_CREATE             0x00000004UL
#define RL_PASS               0x00000008UL
#define RL_CHANGE             0x00000010UL
#define RL_MMAP_WRITE         0x00000020UL
#define RL_ATTACH             0x00000040UL
#define RL_ASSOCIATE          0x00000080UL
#define RL_BIND               0x00000100UL
#define RL_CONNECT            0x00000200UL
#define RL_LISTEN             0x00000400UL
#define RL_ACCEPT             0x00000800UL
#define RL_OPEN               0x00001000UL
#define RL_PARENT             0x00002000UL
#define RL_VERSION            0x00004000UL
#define RL_LINK               0x00008000UL
#define RL_NAMED              0x00010000UL
#define RL_IFC                0x00020000UL
#define RL_EXEC               0x00040000UL
#define RL_FORK               0x00080000UL
#define RL_UNKNOWN            0x00100000UL
#define RL_VERSION_PROCESS    0x00200000UL
#define RL_SEARCH             0x00400000UL
#define RL_ALLOWED            0x00800000UL
#define RL_DISALLOWED         0x01000000UL
#define RL_MMAP_READ          0x02000000UL
#define RL_MMAP_EXEC          0x04000000UL

#define FLOW_ALLOWED        1
#define FLOW_DISALLOWED     0

#define NODE_TRACKED        1
#define NODE_NOT_TRACKED    0

#define NODE_PROPAGATE      1
#define NODE_NOT_PROPAGATE  0

#define NODE_RECORDED       1
#define NODE_UNRECORDED     0

#define NODE_INITIALIZED     1
#define NODE_NOT_INITIALIZED 0

#define NAME_RECORDED       1
#define NAME_UNRECORDED     0

#define NODE_OPAQUE         1
#define NODE_NOT_OPAQUE     0

#define INODE_LINKED        1
#define INODE_UNLINKED      0

#define STR_MAX_SIZE        128

#define node_kern(prov) ((prov)->node_info.node_kern)
#define prov_type(prov) (prov)->node_info.identifier.node_id.type
#define node_identifier(node) (node)->node_info.identifier.node_id
#define relation_identifier(relation) (relation)->relation_info.identifier.relation_id

struct node_identifier{
  uint32_t type;
  uint64_t id;
  uint32_t boot_id;
  uint32_t machine_id;
  uint32_t version;
};

struct relation_identifier{
  uint32_t type;
  uint64_t id;
  uint32_t boot_id;
  uint32_t machine_id;
};

#define PROV_IDENTIFIER_BUFFER_LENGTH sizeof(struct node_identifier)

typedef union prov_identifier{
  struct node_identifier node_id;
  struct relation_identifier relation_id;
  uint8_t buffer[PROV_IDENTIFIER_BUFFER_LENGTH];
} prov_identifier_t;

// TODO use a single byte
// TODO add taint tracking
struct node_kern{
  uint8_t recorded;
  uint8_t name_recorded;
  uint8_t tracked;
  uint8_t opaque;
  uint8_t propagate;
  uint8_t initialized;
};

struct msg_struct{
  prov_identifier_t identifier;
};

struct relation_struct{
  prov_identifier_t identifier;
  uint32_t type;
  uint8_t allowed;
  prov_identifier_t snd;
  prov_identifier_t rcv;
};

struct node_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
};

struct task_prov_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  uint32_t uid;
  uint32_t gid;
};

struct inode_prov_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  uint32_t uid;
  uint32_t gid;
  uint16_t mode;
  uint8_t sb_uuid[16];
};

struct sb_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  uint8_t uuid[16];
};

struct msg_msg_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  long type;
};

struct shm_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  uint16_t mode;
};

struct sock_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  uint16_t type;
  uint16_t family;
  uint8_t protocol;
};

typedef union prov_msg{
  struct msg_struct           msg_info;
  struct relation_struct      relation_info;
  struct node_struct          node_info;
  struct task_prov_struct     task_info;
  struct inode_prov_struct    inode_info;
  struct msg_msg_struct       msg_msg_info;
  struct shm_struct           shm_info;
  struct sock_struct          sock_info;
  struct sb_struct            sb_info;
} prov_msg_t;

struct str_struct{
  prov_identifier_t identifier;
  size_t length;
  char str[PATH_MAX];
};

struct file_name_struct{
  prov_identifier_t identifier;
  size_t length;
  char name[PATH_MAX];
};

struct address_struct{
  prov_identifier_t identifier;
  struct sockaddr addr;
  size_t length;
};

struct ifc_context_struct{
  prov_identifier_t identifier;
  struct ifc_context context;
};

struct disc_node_struct{
  prov_identifier_t identifier;
  struct node_kern node_kern;
  size_t length;
  char content[PATH_MAX];
  prov_identifier_t parent;
};

typedef union long_msg{
  struct msg_struct           msg_info;
  struct node_struct          node_info;
  struct str_struct           str_info;
  struct file_name_struct     file_name_info;
  struct address_struct       address_info;
  struct ifc_context_struct   ifc_info;
  struct disc_node_struct     disc_node_info;
} long_prov_msg_t;

struct prov_filter{
  uint32_t filter;
  uint8_t add;
};

#define PROV_SET_TRACKED		  1
#define PROV_SET_OPAQUE 		  2
#define PROV_SET_PROPAGATE    3

struct prov_file_config{
  char name[PATH_MAX];
  struct inode_prov_struct prov;
  uint8_t op; // on write
};

#endif
