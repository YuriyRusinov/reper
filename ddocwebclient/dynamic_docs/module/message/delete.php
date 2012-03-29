<?
$page_title = 'Удаление записи журнала';

$errors = array();
$id = intval(get_request_variable('id', 0));
$_REQUEST['callback'] = get_request_variable('callback', '');

$journal = new journal();
$journal->id = $id;
if ($id > 0) {
    if ($journal->Load()) {
        $errors = $journal->IsValidData();
        if (!$errors) {
            $journal->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
