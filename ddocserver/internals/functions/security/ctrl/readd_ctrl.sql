--this MUST be invoked first!!
\i ./functions/security/ctrl/trg_acl_ioctrl.sql

\i ./functions/security/ctrl/acl_securetable.sql
\i ./functions/security/ctrl/acl_secure_io_objects.sql
\i ./functions/security/ctrl/acl_secure_attr_values.sql

--invoked in ./initprivs.sql
--\i ./functions/security/ctrl/acl_secure_io_objects_apply.sql
--\i ./functions/security/ctrl/acl_secure_attr_values_apply.sql
