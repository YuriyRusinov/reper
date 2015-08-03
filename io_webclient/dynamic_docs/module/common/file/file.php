<?
$include_header_footer = intval(get_request_variable('ajax-request', 0))===1 ? false:true;
$item_id = intval(get_request_variable('id', 0));

$type = get_request_variable('type', '');

$db = db_class::get_instance();

$legend_title = 'Файлы';
$enable = false;

if ($item_id>0 && $type=='project') {
    $legend_title = 'Файлы проекта';
    $project = new project();
    $project->id_project = $item_id;
    if ($project->Load() && $project->CheckActionEnable($STORAGE, 'file')) $enable = true;
    $obj2attach = &$project;
}

if ($item_id>0 && $type=='stage') {
    $legend_title = 'Файлы этапа';

    $stage = new stage();
    $stage->id_stage = $item_id;
    if ($stage->Load() && $stage->CheckActionEnable($STORAGE, 'file')) $enable = true;
    $obj2attach = &$stage;
}

if ($item_id>0 && $type=='task') {
    $legend_title = 'Файлы задачи';

    $task = new task();
    $task->id_task = $item_id;
    if ($task->Load() && $task->CheckActionEnable($STORAGE, 'file')) $enable = true;
    $obj2attach = &$task;
}


$show_files = array();
if ($include_header_footer) include SITE_FILE_ROOT.'template/simple_header.php';

if ($enable) {
    $query = '
                SELECT * FROM "'.VIEW_FILE_ATTACH.'"
                WHERE
                    "type"=\''.$db->str2base($type).'\' AND
                    "item_id" = \''.$db->str2base($item_id).'\'
            ';
    $db->query($query);
    $files = $db->value;

    $role_ids = array();
    $is_admin = true;
    if (!check_current_employee_admin($STORAGE)) {
        $is_admin = false;
        $role_ids = get_current_epmloyee_role_ids($STORAGE);
    }
    for ($i=0;$i<count($files);$i++) {
        if (!$is_admin) {
            if (count($role_ids)==0) break;

            $query = 'SELECT "file_id" FROM '.TABLE_FILE_ROLE.' WHERE "file_id"='.$files[$i]['file_id'].' AND role_id IN ('.join(',', $role_ids).') LIMIT 1';
            $db->query($query);
            if (count($db->value)==0) continue;
        }

        $show_files[] = $files[$i];
    }

    include SITE_FILE_ROOT.'template/common/file/file.php';
}


if ($include_header_footer) include SITE_FILE_ROOT.'template/simple_footer.php';
