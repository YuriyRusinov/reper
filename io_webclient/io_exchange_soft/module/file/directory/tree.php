<?
header("Content-Type: text/xml; charset=utf-8");

$id = get_request_variable('id', 'root');
$complete_flag = '';

$id = html_entity_decode(preg_replace('~%(u[a-f\d]{4}|[a-f\d]{2})~ie','"&#".hexdec("$1").";"',$id),ENT_NOQUOTES,"utf-8");
$id = str_replace('..', '', $id);

$db = db_class::get_instance();

echo '<?xml version="1.0" encoding="UTF-8"?>'."\n";
echo '<tree id="'.escape($id).'">'."\n";

$id = mb_convert_encoding($id, 'windows-1251', 'utf-8');
file_put_contents('C:\Temp\aaa.txt', SITE_PROJECT_FILE_ROOT.$id);

if ($id==='root') {
    if (access_enable($STORAGE, 'project', 'view')) {
        $status = 'active';
        $where = array();
        $where[] = '"status"=\''.$db->str2base($status).'\'';

        $query = 'SELECT * FROM "'.TABLE_PROJECT.'"';

        if ($complete_flag!='') $where[] = '"complete_flag"=\''.db_class::str2base($complete_flag).'\'';
        if (!check_current_employee_admin($STORAGE)) {
            $query.=' LEFT JOIN "'.TABLE_PROJECT_EMPLOYEE.'" ON "id_project"="project_id"';
            $where[] = '"employee_id"='.$STORAGE['current_employee']->id_employee;
        }

        if (count($where)>0) $query.=' WHERE '.join(' AND ', $where);
        $db->query($query);
        $projects = $db->value;

        for ($i=0;$i<count($projects);$i++) {
            $childs = false;

            if (file_exists(SITE_PROJECT_FILE_ROOT.$projects[$i]['id_project']) && is_dir(SITE_PROJECT_FILE_ROOT.$projects[$i]['id_project'])) {
                $files = scandir(SITE_PROJECT_FILE_ROOT.$projects[$i]['id_project']);
                if (count($files)>2) $childs = true;
            }

            echo '<item child="'.($childs ? 1:0).'" id="'.$projects[$i]['id_project'].'/" text="'.escape($projects[$i]['name']).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
            echo '<userdata name="project_id">'.$projects[$i]['id_project'].'</userdata>';
            echo '</item>';
        }
    }
} else {
    $project_id = 0;
    if (preg_match('/(\d+)\/(.*)/', $id, $matches)) {
        $project_id = $matches[1];
    }

    $access_enable = true;
    if ($project_id>0) {
        if (!check_current_employee_admin($STORAGE)) {
            $query = 'SELECT COUNT(*) AS num FROM "'.TABLE_PROJECT_EMPLOYEE.'" WHERE "employee_id"='.$STORAGE['current_employee']->id_employee;
            $db->query($query);

            if (intval($db->value[0]['num'])==0) $access_enable = false;
        }
    }

    if ($project_id>0 && $access_enable) {
        if (file_exists(SITE_PROJECT_FILE_ROOT.$id)) {
            $file = SITE_PROJECT_FILE_ROOT.$id;
            $files = scandir($file);

            while(list(,$filename)=each($files)) {
                if ($filename=='.' || $filename=='..') continue;
                $current_file = $file.$filename;
                $current_ref_file = $id.$filename;
                $current_http_file = SITE_PROJECT_HTTP_ROOT.$id.$filename;

                if (is_dir($current_file)) {
                    $childs = (count(scandir($current_file.'/'))>2);
                    echo '<item child="'.($childs ? 1:0).'" id="'.escape(mb_convert_encoding($current_ref_file, 'utf-8', 'windows-1251').'/').'" text="'.escape(mb_convert_encoding($filename, 'utf-8', 'windows-1251')).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
                    echo '<userdata name="isfile">0</userdata>';
                    echo '<userdata name="project_id">'.$project_id.'</userdata>';
                    echo '</item>';
                } else {
                    $childs = false;
                    echo '<item child="'.($childs ? 1:0).'" id="'.escape(mb_convert_encoding($current_ref_file, 'utf-8', 'windows-1251')).'" text="'.escape(mb_convert_encoding($filename, 'utf-8', 'windows-1251')).'">';
                    echo '<userdata name="isfile">1</userdata>';
                    echo '<userdata name="project_id">'.$project_id.'</userdata>';
                    echo '</item>';
                }
            }
        }
    }
}

echo '</tree>'."\n";


