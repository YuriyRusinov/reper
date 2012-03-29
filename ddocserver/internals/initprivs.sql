\encoding win1251

select setGrantsEx(NULL);

\i ./functions/security/ctrl/acl_secure_io_objects_apply.sql
\i ./functions/security/ctrl/acl_secure_attr_values_apply.sql

\i ./functions/rubricator/triggers/trgrubric_acc.sql

