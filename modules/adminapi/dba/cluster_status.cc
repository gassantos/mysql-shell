/*
 * Copyright (c) 2018, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2.0,
 * as published by the Free Software Foundation.
 *
 * This program is also distributed with certain software (including
 * but not limited to OpenSSL) that is licensed under separate terms, as
 * designated in a particular file or component or in included license
 * documentation.  The authors of MySQL hereby grant you an additional
 * permission to link the program and your derivative works with the
 * separately licensed software that they have included with MySQL.
 * This program is distributed in the hope that it will be useful,  but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
 * the GNU General Public License, version 2.0, for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "modules/adminapi/dba/cluster_status.h"
#include "modules/adminapi/dba/replicaset_status.h"
#include "modules/adminapi/mod_dba_common.h"
#include "mysqlshdk/libs/mysql/group_replication.h"

namespace mysqlsh {
namespace dba {

Cluster_status::Cluster_status(Cluster *cluster,
                               mysqlshdk::utils::nullable<bool> extended,
                               mysqlshdk::utils::nullable<bool> query_member)
    : m_cluster(cluster), m_extended(extended), m_query_members(query_member) {}

Cluster_status::~Cluster_status() {}

void Cluster_status::prepare() {}

shcore::Value Cluster_status::get_replicaset_status(
    const std::shared_ptr<ReplicaSet> &replicaset) {
  shcore::Value ret;

  // Create the ReplicaSet Status command and execute it.
  Replicaset_status op_replicaset_status(replicaset, m_extended,
                                         m_query_members);
  // Always execute finish when leaving "try catch".
  auto finally = shcore::on_leave_scope(
      [&op_replicaset_status]() { op_replicaset_status.finish(); });
  // Prepare the ReplicaSet Status command execution (validations).
  op_replicaset_status.prepare();
  // Execute Status operations.
  ret = op_replicaset_status.execute();

  return ret;
}

shcore::Value Cluster_status::execute() {
  shcore::Dictionary_t dict = shcore::make_dict();

  (*dict)["clusterName"] = shcore::Value(m_cluster->get_name());

  // Get the default replicaSet status
  {
    shcore::Value ret;

    std::shared_ptr<ReplicaSet> default_rs =
        m_cluster->get_default_replicaset();

    ret = get_replicaset_status(default_rs);

    (*dict)["defaultReplicaSet"] = shcore::Value(ret);
  }

  // Iterate all replicasets and get the status for each one

  mysqlshdk::mysql::Instance target_instance(m_cluster->get_group_session());

  std::string addr = target_instance.get_canonical_address();
  (*dict)["groupInformationSourceMember"] = shcore::Value(addr);

  // metadata server, if its a different one
  if (m_cluster->metadata()->get_session() != m_cluster->get_group_session()) {
    auto mdsession = m_cluster->metadata()->get_session();
    mysqlshdk::mysql::Instance md_instance(mdsession);
    (*dict)["metadataServer"] =
        shcore::Value(md_instance.get_canonical_address());
  }

  return shcore::Value(dict);
}

void Cluster_status::rollback() {
  // Do nothing right now, but it might be used in the future when
  // transactional command execution feature will be available.
}

void Cluster_status::finish() {}

}  // namespace dba
}  // namespace mysqlsh
