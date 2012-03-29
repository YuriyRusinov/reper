<?
$page_title = 'Настройка отображаемых столбцов';

//
$id = intval(get_request_variable('id', 0));
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');
	
$rows_conf = new rows_conf;
$rows_conf->id_employee = $STORAGE['current_employee']->id_employee;
$rows_conf->table_name = 'employee';
$rows_conf->Load();

if($save)
{
	$id = get_request_variable('id', 'no');
	$active_flag = get_request_variable('active_flag', 'no');
	$photo = get_request_variable('photo', 'no');
	$name = get_request_variable('name', 'no');
	$department = get_request_variable('department', 'no');
	$post = get_request_variable('post', 'no');
	$email = get_request_variable('email', 'no');
	$office_phone = get_request_variable('office_phone', 'no');
	$room = get_request_variable('room', 'no');

    $columns = '';

    if($id  == 'no')
    {
    	$columns = $columns.'id';
    	$columns = $columns.',';
    }

    if($active_flag  == 'no')
    {
    	$columns = $columns.'active_flag';
    	$columns = $columns.',';
    }

    if($photo  == 'no')
    {
    	$columns = $columns.'photo';
    	$columns = $columns.',';
    }

    if(false)//($name  == 'no')
    {
    	$columns = $columns.'name';
    	$columns = $columns.',';
    }

    if($department  == 'no')
    {
    	$columns = $columns.'department';
    	$columns = $columns.',';
    }

    if($post  == 'no')
    {
    	$columns = $columns.'post';
    	$columns = $columns.',';
    }

    if($email  == 'no')
    {
    	$columns = $columns.'email';
    	$columns = $columns.',';
    }

    if($office_phone  == 'no')
    {
    	$columns = $columns.'office_phone';
    	$columns = $columns.',';
    }

    if($room  == 'no')
    {
    	$columns = $columns.'room';
    	$columns = $columns.',';
    }

	//delete last symbol

	$rows_conf->columns = $columns;

    $errors = $rows_conf->IsValidData();
    if (count($errors)==0) {
        $db = db_class::get_instance();
        $db->begin();
        $rows_conf->Save();
        $db->commit();
    }
    
}
else
{
 	$columns = $rows_conf->columns;

    $id = 'yes';
	$active_flag = 'yes';
	$photo = 'yes';
	$name = 'yes';
	$department = 'yes';
	$post = 'yes';
	$email = 'yes';
	$office_phone = 'yes';
    $room = 'yes';
    
	if(strlen($columns) > 0)
	{
		if(substr_count($columns,'id') > 0)
		{
			$id = 'no';
		}
		if(substr_count($columns,'active_flag') > 0)
		{
			$active_flag = 'no';
		}
		if(substr_count($columns,'photo') > 0)
		{
			$photo = 'no';
		}
		if(substr_count($columns,'name') > 0)
		{
			$name = 'no';
		}
		if(substr_count($columns,'department') > 0)
		{
			$department = 'no';
		}
		if(substr_count($columns,'post') > 0)
		{
			$post = 'no';
		}
		if(substr_count($columns,'email') > 0)
		{
			$email = 'no';
		}
		if(substr_count($columns,'office_phone') > 0)
		{
			$office_phone = 'no';
		}

		if(substr_count($columns,'room') > 0)
		{
			$room = 'no';
		}
	}

}


include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';