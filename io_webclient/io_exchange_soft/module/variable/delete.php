<?
$page_title = 'Удаление дополнительного поля';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$variable = new variable();
$variable->id_variable = $id;
if ($id > 0) {
    if ($variable->Load()) {
        $errors = $variable->IsValidData();
        if (!$errors) {
            $variable->Delete();

            $db = db_class::get_instance();
            $query = 'CALL create_'.str_replace('-', '_', $variable->module).'_variable_view ()';
            $db->query($query);
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
