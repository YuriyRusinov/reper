<?
$page_title = 'Редактирование должности';

$id = intval(get_request_variable('id', 0));
$name = get_request_variable('name', '');
$remark = get_request_variable('remark', '');

$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$office = new office();
$office->id_office = $id;
if ($id > 0) {
    if (!$office->Load()) {
        $office->id_office = 0;
    }
}

if ($save) {
    $office->name = $name;
    $office->remark = $remark;
    $errors = $office->IsValidData();

    if (count($errors)==0) {
        $db = db_class::get_instance();
        $db->begin();

        $office->Save();

       

        $db->commit();
    }
} 



include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
