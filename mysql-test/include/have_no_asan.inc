## skip asan
--let $_outfile= $MYSQLTEST_VARDIR/tmp/is_asan.tmp
--error 0,1,127
--exec $MYSQLD  -V | grep asan | wc -l > $_outfile 2>&1
disable_query_log;
--eval SET @is_asan = LOAD_FILE('$_outfile')
enable_query_log;
--remove_file $_outfile
if (`SELECT @is_asan = 1`)
{
   --skip Test requires 'no_asan'
}
