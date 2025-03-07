# ==== Purpose ====
# initialize a new learner node
#
# ==== usage ====
# [--let $learner_base_port= $port + 40]
# [--let $learner_cluster_id= 1]
# --source include/start_new_learner.inc

--echo # start a new learner

if (!$learner_base_port)
{
	--echo # learner_base_port not passed, exit
	--die must set learner_base_port
}
if (!$learner_cluster_id)
{
	let $learner_cluster_id= 1;
}

--let $learner_datadir= $MYSQL_TMP_DIR/learner_$learner_base_port
--let $learner_logdir= $MYSQL_TMP_DIR/learner_$learner_base_port.log
--let $learner_pid_file= $learner_datadir.pid
--let $learner_server_id= $learner_base_port

--let $learner_port= $learner_base_port
let $learner_x_port = query_get_value("select $learner_base_port+10 as c1", c1, 1);
let $learner_galaxyx_port = query_get_value("select $learner_base_port+20 as c1", c1, 1);
let $learner_rpc_port = query_get_value("select $learner_base_port+30 as c1", c1, 1);
let $learner_consensus_port = query_get_value("select $learner_base_port+40 as c1", c1, 1);

--let $learner_cluster_info= '127.0.0.1:$learner_consensus_port'

--let $learner_socket_path= $learner_datadir.sock
--let BASEDIR= `select @@basedir`
--let DDIR= $learner_datadir
--let MYSQLD_LOG= $learner_logdir

--let extra_args=--defaults-file='$MYSQLTEST_VARDIR/my.cnf' --console --skip-ssl --basedir=$BASEDIR --lc-messages-dir=$MYSQL_SHAREDIR --socket=$learner_socket_path --mysqlx-port=$learner_x_port --rpc-port=$learner_rpc_port --port=$learner_port --pid-file=$learner_pid_file --new-rpc=0 --mysqlx=0

--let init_args=--server_id=$learner_server_id --cluster-id=$learner_cluster_id --cluster-learner-node=ON --cluster-info=$learner_cluster_info 

--exec if [ -d $DDIR ]; then rm -rf $DDIR; fi

--exec $MYSQLD $extra_args --initialize-insecure --datadir=$DDIR $init_args > $MYSQLD_LOG 2>&1

--exec $MYSQLD $extra_args --datadir=$DDIR $init_args >> $MYSQLD_LOG 2>&1 &

--sleep 20
