<?
$page_title = 'Редактирование дополнительного поля';

$id = intval(get_request_variable('id', 0));
$module = get_request_variable('module', '');
$name = get_request_variable('name', '');
$show_name = get_request_variable('show_name', '');
$required = get_request_variable('required', 'no')=='yes' ? 'yes':'no';
$type = get_request_variable('type', '');
$format = get_request_variable('format', '');
$error_message = get_request_variable('error_message', '');
$order = intval(get_request_variable('order', 0));
$list_show = get_request_variable('list_show', 'no')=='yes' ? 'yes':'no';
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$variable = new variable();
$variable->id_variable = $id;
if ($id > 0) {
    if (!$variable->Load()) {
        $variable->id_variable = 0;
    }
}

if ($save) {
    $variable->module = $module;
    $variable->name = $name;
    $variable->show_name = $show_name;
    $variable->required = $required;
    $variable->type = $type;
    $variable->format = $format;
    $variable->error_message = $error_message;
    $variable->order = $order;
    $variable->list_show = $list_show;

    $errors = $variable->IsValidData();
    if (count($errors)==0) {
        $variable->Save();

        $db = db_class::get_instance();
        $query = 'CALL create_'.str_replace('-', '_', $variable->module).'_variable_view ()';
        $db->query($query);
    }
} else {
    if ($variable->id_variable == 0) {
        $variable->order = $variable->GetNextOrder();
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
