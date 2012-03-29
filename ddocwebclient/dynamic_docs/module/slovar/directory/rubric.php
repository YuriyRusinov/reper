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

$io_id = get_request_variable('io_id',0);

$db = db_class::get_instance();

echo '<?xml version="1.0" ?>'."\n";
echo '<tree id="'.$id.'">'."\n";
switch ($type) {
    case 'root':
        $rubricator = new rubricator();
        //load root rubric for this io_object
        
        if($io_id > 0)
        {
        	$query = "SELECT id FROM iogetrubrics($io_id) where id_parent is null";
        	$db->query($query);
        	$num = $db->value[0]['id'];
        	$rubricator->id = $num;
        	
        	$rubricator->Load();
        	
        }
        else 
        {
        	$rubricator->id = null;
        }
        $childs = $rubricator->ChildExists();
        
        if($io_id > 0)
        {
        	if($rubricator->id > 0)
        	{
        	
        		if($rubricator->id_io_category > 0)
        		{
        		  echo '<item text="'.$rubricator->name.'" id="foldercategory-'.$rubricator->id.'" child="'.($childs ? 1:0).'"></item>';
        		}
        		else 
        		{
        		  echo '<item text="'.$rubricator->name.'" id="folder-'.$rubricator->id.'" child="'.($childs ? 1:0).'"></item>';	
        		}
        	
        	}
        	else 
        	{
        		echo '<item text="Рубрик не обнаружено:" id="io-'.$io_id.'" child="0"></item>';
        	}
        	
        }
        else 
        {
        	echo '<item text="Рубрики:" id="folder-0" child="'.($childs ? 1:0).'"></item>';
        }
    break;
    case 'folder':
    case 'foldercategory':	
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
            
            if($rubricator->id_io_category > 0)
            {
             	echo '<item child="'.($childs ? 1:0).'" id="foldercategory-'.$rubricator->id.'" text="'.escape($type_collection->_collection[$i]->name).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
            }
            else 
            {
            	echo '<item child="'.($childs ? 1:0).'" id="folder-'.$rubricator->id.'" text="'.escape($type_collection->_collection[$i]->name).'" im0="folderOpen.gif" im1="folderOpen.gif" im2="folderClosed.gif">';
            }
            
            
            
             if($rubricator->ObjectExists())
            {
            	if($type_id > 0)
            	{
            		//for rubric with id_category
            		if($rubricator->id_io_category > 0)
            		{
            			
            		echo '<item child="0" id="category-'.$rubricator->id.'" text="Просмотреть рубрику"  im0="../csh_books/leaf.gif"   ></item>';
            			
            			
            		}
            		//other rubrics
            		else 
            		{
            		 	$agr_collection = new collection();
            		 	$agr_collection->Load(TABLE_IO_RUBRICATOR, false, 'id_rubric='.$rubricator->id, 'id_rubric ASC');
            
             		for ($k=0;$k<count($agr_collection->_collection);$k++) {
             			
             			$name = '';
             			if($agr_collection->_collection[$k]->id_io_object > 0)
             			{
             				
             				$io = new io_objects();
             				$io->id = $agr_collection->_collection[$k]->id_io_object;
             				
             				if($io->Load())
             				{
             					$name = $io->name;
             				}
             			}
             			
                		echo '<item child="0" id="agreement-'.$agr_collection->_collection[$k]->id_io_object.'" text="'.escape($name).'"  im0="../csh_books/leaf.gif"   ></item>';
            			}
            		}
            	
            	
            	}
            }                        
             echo '</item>';
        }           
    break;
    
    case 'agreement':
    
    break;
    
    /*case 'category':
    	
    break;*/
    
    case 'io':
    
    break;
    
}
echo '</tree>'."\n";
