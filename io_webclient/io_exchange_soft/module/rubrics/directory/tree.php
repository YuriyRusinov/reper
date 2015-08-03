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
        $rubricator = new rubricator();
        $rubricator->id = null;
        $childs = $rubricator->ChildExists();
        echo '<item text="Рубрики:" id="folder-0" child="'.($childs ? 1:0).'"></item>';
    break;
    case 'folder':
        $type_collection = new collection();
        $type_collection->Load(TABLE_RUBRICATOR, false, ($type_id > 0 ? 'id_parent='.$type_id : 'id_parent IS NULL AND id_io_object IS NULL'), 'name ASC');
        $rubricator = new rubricator();
        for ($i=0;$i<count($type_collection->_collection);$i++) {
            $rubricator->id = $type_collection->_collection[$i]->id;
            $rubricator->Load();
            
            $childs = $rubricator->ObjectExists();
            if(!$childs)
            {
            	$childs = $rubricator->ChildExists();
            }
            
            
             	echo '<item child="'.($childs ? 1:0).'" id="folder-'.$rubricator->id.'" text="'.escape($type_collection->_collection[$i]->name).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
            
            
            
             if($rubricator->ObjectExists())
            {
            	if($type_id > 0)
            	{
            		$agr_collection = new collection();
            		$agr_collection->Load(TABLE_IO_RUBRICATOR, false, 'id_rubric='.$rubricator->id, 'id_rubric ASC');
            
             		for ($k=0;$k<count($agr_collection->_collection);$k++) {
             			
             			$io_object = new io_objects();
             			$io_object->id = $agr_collection->_collection[$k]->id_io_object;
             			$io_object->Load();
             			
                		echo '<item child="0" id="agreement-'.$agr_collection->_collection[$k]->id_io_object.'" text="'.escape($io_object->name).'"  im0="../csh_books/leaf.gif"   ></item>';
            		}
            	}
            }                        
             echo '</item>';
        }           
    break;
    
    case 'agreement':
    
    break;
    
}
echo '</tree>'."\n";
