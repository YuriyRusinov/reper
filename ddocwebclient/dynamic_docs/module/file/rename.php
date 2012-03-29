<?
$page_title = 'Переименовывание каталогов/файлов';

$source = preg_replace('/[\/]{2,}/', '/', get_request_variable('source', ''));
$destination = preg_replace('/[\/]{2,}/', '/', get_request_variable('destination', ''));
$name = basename($source);

$source_win1251 = mb_convert_encoding($source, 'windows-1251', 'utf-8');
$source_win1251 = SITE_PROJECT_FILE_ROOT.$source_win1251;
$source_win1251 = str_replace('..', '', $source_win1251);

$destination_win1251 = mb_convert_encoding($destination, 'windows-1251', 'utf-8');
$destination_win1251 = SITE_PROJECT_FILE_ROOT.$destination_win1251;
$destination_win1251 = str_replace('..', '', $destination_win1251);

$errors = array();

if (file_exists($source_win1251)) {
    if (is_file($source_win1251)) {
        $file = new file();
        if ($file->LoadByCrc(crc($source))) {
            $file->name = basename($destination);
            $file->path = dirname($destination);
            $file->Save();
        }
    }
    rename($source_win1251, $destination_win1251);
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
