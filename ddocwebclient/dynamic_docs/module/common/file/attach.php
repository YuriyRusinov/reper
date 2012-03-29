<?
$page_title = 'Присоединение файлов';

$errors = array();
$attach_files = get_request_variable('attach_files', array());
$disable_attach_files = get_request_variable('disable_attach_files', array());

$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');

$db = db_class::get_instance();

$files = array();
$files_ids = array();
if ($obj2attach->CheckActionEnable($STORAGE, 'file-attach')) {
    $access_enable = true;

    $roles = get_current_epmloyee_role_ids($STORAGE);

    if (count($roles)>0 || check_current_employee_admin($STORAGE)) {
        // Получаем все доступные файлы
        $query = '
                    SELECT DISTINCT "id_file", "name", "path"
                    FROM "'.VIEW_FILE_ROLE.'"
                    WHERE
                 ';

        if (!check_current_employee_admin($STORAGE)) {
            $query .= ' role_id IN ('.join(',', $roles).') AND ';
        }

        $query .= '
                    path LIKE \''.$db->str2base($project->id_project).'/%\'
                    ORDER BY name
                  ';

        $db->query($query);
        $files = $db->value;
        $files_ids = get_values_array($files, 'id_file');
    }


    if (!$save) {
        $query = '
                    SELECT id_file FROM "'.VIEW_FILE_ATTACH.'"
                    WHERE
                        "type"=\''.$db->str2base($type).'\' AND
                        "item_id" = \''.$db->str2base($id).'\'
                    ORDER BY name
                ';
        $db->query($query);
        $attach_files = get_values_array($db->value, 'id_file');
    } else {
        // Удаляем невыделенные файлы
        $file_attach = new file_attach();
        $file_attach->type = $type;
        $file_attach->item_id = $id;

        for ($i=0;$i<count($disable_attach_files);$i++) {
            $disable_attach_files[$i] = intval($disable_attach_files[$i]);
            if ($disable_attach_files[$i]>0) {
                if (array_search($disable_attach_files[$i] ,$files_ids)===false) continue;

                $file_attach->file_id = $disable_attach_files[$i];
                $file_attach->Delete();
            }
        }

        // Сохраняем добавленные файлы
        for ($i=0;$i<count($attach_files);$i++) {
            $attach_files[$i] = intval($attach_files[$i]);
            if ($attach_files[$i]>0) {
                if (array_search($attach_files[$i] ,$files_ids)===false) continue;

                $file_attach->file_id = $attach_files[$i];
                $file_attach->Save();
            }
        }
    }
} else {
    $access_enable = false;
}

$attach_files = array_flip($attach_files);

include SITE_FILE_ROOT.'template/simple_header.php';
if ($access_enable) {
    include SITE_FILE_ROOT.'template/common/file/attach.php';
} else {
    include SITE_FILE_ROOT.'template/accesserror.php';
}
include SITE_FILE_ROOT.'template/simple_footer.php';
