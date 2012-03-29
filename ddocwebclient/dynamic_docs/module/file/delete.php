<?
$page_title = 'Удаление каталогов/файлов';

$source = preg_replace('/[\/]{2,}/', '/', get_request_variable('source', ''));
$source_win1251 = mb_convert_encoding($source, 'windows-1251', 'utf-8');
$source_win1251 = SITE_PROJECT_FILE_ROOT.$source_win1251;
$source_win1251 = str_replace('..', '', $source_win1251);

$errors = array();

if (file_exists($source_win1251)) {
    if (is_dir($source_win1251)) {
        rmdir($source_win1251);
    } else {
        unlink($source_win1251);
        $file = new file();
        if ($file->LoadByCrc(crc($source))) {
            $file->Delete();
        }
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
