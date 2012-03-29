<?
$page_title = 'Добавление директории';

$destination = get_request_variable('destination', '');
$name = get_request_variable('dir', '');

$destination_win1251 = mb_convert_encoding($destination, 'windows-1251', 'utf-8');
$destination_win1251 = $destination_win1251;
$destination_win1251 = str_replace('..', '', $destination_win1251);

$name_win1251 = mb_convert_encoding($name, 'windows-1251', 'utf-8');
$name_win1251 = $name_win1251;
$name_win1251 = str_replace('..', '', $name_win1251);

$source_win1251 = SITE_PROJECT_FILE_ROOT.$destination_win1251.'/'.$name_win1251;

$errors = array();

if (!file_exists($source_win1251)) {
    mkdir($source_win1251);
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
