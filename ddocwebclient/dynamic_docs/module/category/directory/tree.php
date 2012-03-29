<?
header("Content-type:text/xml");

$id = get_request_variable('id', 'root');
if ($id!='root') {
    list($type, $type_id) = explode('-', $id);
} else {
    $type = 'root';
    $type_id = 0;
}
$type_id = intval($type_id);


$db = db_class::get_instance();

echo '<?xml version="1.0" ?>'."\n";
echo '<tree id="'.$id.'">'."\n";
switch ($type) {
    case 'root':
        $categories = new io_categories();
        $categories->id = null;
        $childs = true;
        echo '<item text="Категории и шаблоны:" id="folder-0" child="'.($childs ? 1:0).'"></item>';
    break;
    case 'folder':
        $type_collection = new collection();
        $type_collection->Load(TABLE_CATEGORIES, false, ($type_id > 0 ? 'id='.$type_id : 'is_main = TRUE'), 'name ASC');
        
        $cat = new io_categories();
        
        for ($i=0;$i<count($type_collection->_collection);$i++) {
        	
            if($type_id <= 0)
            {
        	     		$cat->id = $type_collection->_collection[$i]->id;
            }
            else 
            {
            	 		$cat->id = $type_collection->_collection[$i]->id_child;
            }
            
            $cat->Load();
            
            $childs = $cat->ChildExists();
            
            
            if($cat->id > 0)
            {
            	echo '<item child="'.($childs ? 1:0).'" id="folder-'.$cat->id.'" text="'.escape($cat->name).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';       
            
            	echo '</item>';
            }
                
        }
        
    break;
}
echo '</tree>'."\n";
