<?
$page_title = 'Перенос каталогов/файлов';

$source = preg_replace('/[\/]{2,}/', '/', get_request_variable('source', ''));
$source_win1251 = mb_convert_encoding($source, 'windows-1251', 'utf-8');

$destination = preg_replace('/[\/]{2,}/', '/', get_request_variable('destination', '').'/');
$destination_win1251 = mb_convert_encoding($destination, 'windows-1251', 'utf-8');
$errors = array();

if (file_exists(SITE_PROJECT_FILE_ROOT.$source_win1251)) {
    $file = new file();
    if (is_file(SITE_PROJECT_FILE_ROOT.$source_win1251)) {
        if ($file->LoadByCrc(crc($source)))
        {
            $file->path = $destination;
            $file->Save();
        }
    } else {
        $file->MoveSource($source, $destination);
    }
    smart_move($source_win1251, $destination_win1251, true);
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
