<?
header("Content-type:text/xml");

$id = get_request_variable('id', 'root');
if ($id!='root') $id = intval($id);

$db = db_class::get_instance();

if($id === 0) echo '<?xml version="1.0" ?>'."\n";
if ($id==='root') {
	
    $categories = new io_categories();
    $categories->id = null;
    $childs = $categories->ChildExists();
    echo '<tree id="root">'."\n";
    echo '<item text="'.SITE_NAME.'" id="0" child="1"></item>';
   
    echo '</tree>'."\n";
} else {
    $type_collection = new collection();
    
    
    
    
    $type_collection->Load(TABLE_CATEGORIES, false, ($id > 0 ? 'id='.$id.' AND is_main=TRUE'  : 'is_main=TRUE'), ' name ASC');
    echo '<tree id="'.$id.'">'."\n";
    for ($i=0;$i<count($type_collection->_collection);$i++) {
        $childs = $type_collection->_collection[$i]->ChildExists();
    if($id == 0)    echo '<item child="'.($childs ? 1:0).'" id="'.$type_collection->_collection[$i]->id.'" text="'.escape($type_collection->_collection[$i]->name).'" sys="'.escape($type_collection->_collection[$i]->is_system).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
        
        
	if($childs == true)
	{
		
			$child = $type_collection->_collection[$i]->id_child;
        	
			if($child > 0 && $id > 0)
			{
				$type_coll = new collection();
        		$type_coll->Load(TABLE_CATEGORIES, false,'id='.$child, 'name ASC');
        
        		for ($j=0;$j<count($type_coll->_collection);$j++) {
        
        		echo '<item child="0" id="'.$type_coll->_collection[$j]->id.'" sys="'.escape($type_collection->_collection[$i]->is_system).'" text="'.escape($type_coll->_collection[$j]->name).'" >';
        
        		echo '</item>';
			}
        
        
        
        	}
    	
	}   
      
    if($id ==0)    echo '</item>';
    }
   echo '</tree>'."\n";
    
    
}
