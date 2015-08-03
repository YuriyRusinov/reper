<?
$page_title = 'Редактирование рубрики';

$id = intval(get_request_variable('id', ''));
$parent_id = intval(get_request_variable('parent_id', 0));
$io_id = intval(get_request_variable('io_id', 0));
$kio = get_request_variable('kio', '');


$code = get_request_variable('code', '');
$description = get_request_variable('description', '');
$name = get_request_variable('name', '');
$save = get_request_variable('save', '');
$_REQUEST['callback'] = get_request_variable('callback', '');


$cat_id = intval(get_request_variable('cat_id', 0));
$templ_id = intval(get_request_variable('templ_id', 0));


$rubricator = new rubricator();
$rubricator->id = $id;
if ($id > 0) {
    if (!$rubricator->Load()) {
        $rubricator->id = 0;
    }
}

/*$io_rubric = new collection();
if($rubricator->id > 0)
{
	$io_rubric->Load(TABLE_IO_RUBRICATOR, false, 'id_rubric = '.$rubricator->id);
	$io_rubric = $io_rubric->_collection;
	
	//add new io object to rubrics
	if($io_id > 0)
	{
		$io_rub = new io_rubricator();	
		$io_rub->id_rubric = $id;
		$io_rub->id_io_object = $io_id;
		
		$io_rub->Save();
	}

}*/



if ($save) {
		
	$rubricator->id_io_object = $io_id;
	$rubricator->id_parent = $parent_id;

	$rubricator->id_io_category = $cat_id;
	$rubricator->id_search_template = $templ_id;
	
    if ($rubricator->id_parent === 0) 			$rubricator->id_parent = null;
    if ($rubricator->id_io_object === 0) 		$rubricator->id_io_object = null;
    if ($rubricator->id_io_category === 0) 		$rubricator->id_io_category = null;
    if ($rubricator->id_search_template === 0) 	$rubricator->id_search_template = null;
    
    
    $rubricator->name = $name;
    $rubricator->code = $code;
    $rubricator->description = $description;

    $errors = $rubricator->IsValidData();
    if (count($errors)==0) {
        $rubricator->Save();
    }
}
else 
{
	$categories = new collection();
	$categories->Load(TABLE_CATEGORIES, false, $where, 'name ASC');
	$categories = $categories->_collection;
	
	$templates = new collection();
	$templates->Load(TABLE_SEARCH_TEMPLATES, false, $where, 'name ASC');
	$templates = $templates->_collection;
}




include SITE_FILE_ROOT.'template/header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/footer.php';
