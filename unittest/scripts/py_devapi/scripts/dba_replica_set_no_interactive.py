# Assumptions: ensure_schema_does_not_exist is available
# Assumes __uripwd is defined as <user>:<pwd>@<host>:<plugin_port>
# validateMemer and validateNotMember are defined on the setup script

dba.drop_metadata_schema({'enforce':True})

clusterPassword = 'testing'
#@ Cluster: validating members
cluster = dba.create_cluster('devCluster', clusterPassword)
rset = cluster.get_replica_set()

all_members = dir(rset)

# Remove the python built in members
members = []
for member in all_members:
  if not member.startswith('__'):
    members.append(member)

print "Replica Set Members: %d" % len(members)
validateMember(members, 'name')
validateMember(members, 'get_name')
validateMember(members, 'add_instance')
validateMember(members, 'remove_instance')
validateMember(members, 'help')
validateMember(members, 'rejoin_instance')

#@# Cluster: add_instance errors
rset.add_instance()
rset.add_instance(5,6,7,1)
rset.add_instance(5,5)
rset.add_instance('',5)
rset.add_instance( 5)
rset.add_instance({'host': __host, 'schema': 'abs', 'user': "sample", 'authMethod': 56});
rset.add_instance({'port': __mysql_port_adminapi});
rset.add_instance({'host': __host, 'port': __mysql_port_adminapi}, 'root');

# Cleanup
dba.drop_cluster('devCluster', {"dropDefaultReplicaSet": True})
