<?

$is_enabled_employee_edit=access_enable($STORAGE, $STORAGE['module'], 'edit');
$is_enabled_employee_delete=access_enable($STORAGE, $STORAGE['module'], 'delete');
$is_enabled_rows_conf=access_enable($STORAGE, $STORAGE['module'], 'rows_conf');

include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';
