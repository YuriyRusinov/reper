<div class="pagename">
    <?=$STORAGE['site_path'][count($STORAGE['site_path'])-1]['name']?>
</div>

<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css">
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="department-manipulation">
    
<table width="100%" class="column" cellpadding="0" cellspacing="0" border="0" >
<td> 
<div id="department-toolbar" class="toolbar">
        <button id="add-department" onclick="EditAction('new')"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить новую рубрику">Добавить</button>
        <button id="edit-department" onclick="EditAction('exist')"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать рубрику">Редактировать</button>
        <button id="delete-department" onclick="DeleteAction()"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить выбраную рубрику">Удалить</button>
        
        <button id="tr" onclick=""><img src="<?= SITE_URI_ROOT; ?>img/bg.gif" title="">  </button>
        
        <button id="add-object" onclick="EditActionOb('new')"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить новый объект">Добавить ИО</button>
        <button id="edit-object" onclick="EditActionOb('exist')"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать объект">Редактировать ИО</button>
        <button id="delete-object" onclick="DeleteActionOb()"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить выбраный объект">Удалить ИО</button>
    </div>
    </td>
   </table>
    <div id="department-tree" class="tree"></div>
</div>

<script language="JavaScript">
<!--
function InitButtons(selected_node) {
    document.getElementById('add-department').disabled = false;
    document.getElementById('edit-department').disabled = false;
    document.getElementById('delete-department').disabled = false;

    if (selected_node==null) {
        return;
    }
	else
	{   
    	
				if(selected_node.indexOf("folder") >= 0)
				{
					return;
				}
    		
    			//load all io_s from rubricator
    			if(selected_node.indexOf("category-") >= 0)
    			{
    				var item = '';
    				var j = selected_node.indexOf("category-");
    				item = selected_node.substr(j + 9);
    				
    				if(item > 0)
    				{
    					
    					var url = '<?= SITE_HTTP_ROOT; ?>slovar/category_attrs/id/' + item  + '/callback/Refresh';
    					window.location = url;
    					
    				}
    				
    				
    			}
    			
    		 
    	}
    	    	
   var itemId = ''; 	
	
	if (selected_node.indexOf("agreement-")>=0){
    	//var j = selected_node.indexOf("agreement-");
    	//itemId = selected_node.substr(j + 10);
    	return;
    }
    
    var url = '<?= SITE_HTTP_ROOT; ?>slovar/attrs/id/' + itemId  + '/callback/Refresh';
    
    if(itemId > 0)
    {
   
     	window.location = url;
    
		
	}
    
    
    
    
    
    if (selected_node>=0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('add-department').disabled = false;
                <?
            }
        ?>
    }

    if (selected_node != 0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('edit-department').disabled = false;
                <?
            }

            if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                ?>
                    document.getElementById('delete-department').disabled = false;
                <?
            }
        ?>
    }

}

function EditAction(type) {
    var itemId = tree.getSelectedItemId();
    var parentId = tree.getParentId(itemId);

    
    if(itemId.indexOf("folder-") >= 0 || itemId.indexOf("foldercategory-") >= 0  || itemId.indexOf("io-") >= 0 )
    {
    	
    }
    else
    {
    	return;
    }
    
    
    var io_id = 0;
    if (type=='new') {

    	
    	var type_object = '';
    	
    	
    	if(itemId.indexOf("foldercategory-")>=0)
    	{
    		var j = itemId.indexOf("foldercategory-");
    	    parentId = itemId.substr(j + 15);
    	    
    	}
    	else if (itemId.indexOf("folder-")>=0){
    		var i = itemId.indexOf("folder-");
    		parentId = itemId.substr(i + 7);
    		
    	}
    	else if (itemId.indexOf("io-")>=0){
    		var i = itemId.indexOf("io-");
    		io_id = itemId.substr(i + 3);
    		
    	}

    	itemId = 0;
    	
    }
    else
    {
    	if(itemId.indexOf("foldercategory-")>=0)
    	{
    		var j = itemId.indexOf("foldercategory-");
    	    itemId = itemId.substr(j + 15);
    	}
    	else if (itemId.indexOf("folder-")>=0){
    		var i = itemId.indexOf("folder-");
    		itemId = itemId.substr(i + 7);
    	}
    	
    	if(parentId.indexOf("foldercategory-")>=0)
    	{
    		var j = parentId.indexOf("foldercategory-");
    		parentId = parentId.substr(j + 15);
    	}
    	else if (parentId.indexOf("folder-")>=0){
    		var j = parentId.indexOf("folder-");
    		parentId = parentId.substr(j + 7);
    	}
    	
    	
    	
    }
    
    
    var url = ''; 
        
     url = '<?= SITE_HTTP_ROOT; ?>rubrics/edit/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
    
    var w = 500;
    var h = 200;
    var param = '';
    //OpenPopupWindow(url, w, h, param);
    
     if(parentId !== "root")
    {
     	window.location = url;
    }
    else
    {
    	 url = '<?= SITE_HTTP_ROOT; ?>rubrics/edit/id/0/io_id/' + io_id + '/callback/Refresh';
    	
    	 window.location = url;
    }
    
    
    
    
}

function DeleteAction() {
    var itemId = tree.getSelectedItemId();
    
    
    if(itemId == 'root')
    {
    	alert('Невозможно удалить корневую рубрику.');
    	
    	return;
    }
    
    
    if(itemId.indexOf("folder-") >= 0 || itemId.indexOf("foldercategory-") >= 0)
    {
    	
    }
    else
    {
    	return;
    }
    

    
    if (tree.hasChildren(itemId)>0) {
        alert('Запрещается удалять ветки, содержащие вложения.');
        return;
    }

    if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
        var parentId = tree.getParentId(itemId);
        
        
        
          if(parentId.indexOf("foldercategory-")>=0)
    	{
    		var j = parentId.indexOf("foldercategory-");
    		parentId = parentId.substr(j + 15);
    	}
    	else if (parentId.indexOf("folder-")>=0){
    		var j = parentId.indexOf("folder-");
    		parentId = parentId.substr(j + 7);
    	}	
        
        
        if(itemId.indexOf("foldercategory-")>=0)
    	{
    		var j = itemId.indexOf("foldercategory-");
    	    itemId = itemId.substr(j + 15);
    	}
    	else if (itemId.indexOf("folder-")>=0){
    		var i = itemId.indexOf("folder-");
    		itemId = itemId.substr(i + 7);
    	}
        
        
        
        var url = '<?= SITE_HTTP_ROOT; ?>rubrics/delete/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
        //window.location = url;
    }
}



function DeleteActionOb() {
    
	var itemId = tree.getSelectedItemId();
    var parentId = tree.getParentId(itemId);
       
     if(itemId == null)
    {
    	
    	
    	return;
    }
    
    if(itemId == 'root')
    {
    	alert('Выберите информационный объект!!!');
    	
    	return;
    }
    
    
    if(itemId.indexOf("folder-") >= 0 || itemId.indexOf("foldercategory-") >= 0 || itemId.indexOf("io-") >= 0 )
    {
    	alert('Выберите информационный объект!!!');
    	
    	return;
    	
    }
    
    
     if(parentId.indexOf("foldercategory-")>=0)
    	{
    		var j = parentId.indexOf("foldercategory-");
    		parentId = parentId.substr(j + 15);
    	}
    	else if (parentId.indexOf("folder-")>=0){
    		var j = parentId.indexOf("folder-");
    		parentId = parentId.substr(j + 7);
    	}	
    
    	
    	if(itemId.indexOf("agreement-")>=0)
    	{
    		var j = itemId.indexOf("agreement-");
    	    itemId = itemId.substr(j + 10);
    	    
    	}
    	
//remove object from rubric
    	
    	var url = '<?= SITE_HTTP_ROOT; ?>rubrics/delete_ob/id/' + itemId + '/rubric_id/' + parentId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
       //alert(url);
   
}



function EditActionOb(type){
	
	
	var itemId = tree.getSelectedItemId();
    var parentId = tree.getParentId(itemId);

    if (type=='new') {//add new io object
     
    	if(itemId.indexOf("folder-") >= 0 || itemId.indexOf("foldercategory-") >= 0 )
    	{
  
    		
    		if(parentId.indexOf("foldercategory-")>=0)
    		{
    			var j = parentId.indexOf("foldercategory-");
    			parentId = parentId.substr(j + 15);
    		}
    		else if (parentId.indexOf("folder-")>=0){
    			var j = parentId.indexOf("folder-");
    			parentId = parentId.substr(j + 7);
    		}
    	
             var type_ob = '' ;   	
    		if(itemId.indexOf("foldercategory-")>=0)
    		{
    			var j = itemId.indexOf("foldercategory-");
    			itemId = itemId.substr(j + 15);
    			type_ob = 'category'+ itemId;
    		}
    		else if (itemId.indexOf("folder-")>=0){
    			var j = itemId.indexOf("folder-");
    			itemId = itemId.substr(j + 7);
    			type_ob = 'all';
    		}	
    		
    	var url = '<?= SITE_HTTP_ROOT; ?>slovar/select-object/cat_id/' + itemId  + '/callback/Refresh';
        var w = 500;
        var h = 500;
        var param = '';
        OpenPopupWindow(url, w, h, param);
    		
    		
    		
    			
    	}
    	else
    	{
    		return;
    	}
    	
    	
    	
    	
    	
    	
    	
    }
    else//edit io object
    {
    	
    	
   		if(itemId.indexOf("folder-") >= 0 || itemId.indexOf("foldercategory-") >= 0 || itemId.indexOf("io-") >= 0)
    	{
      		return;	
    	}
    
    	
    	
    	if(itemId.indexOf("agreement-")>=0)
    	{
    		var j = itemId.indexOf("agreement-");
    	    itemId = itemId.substr(j + 10);
    	}
    	
    
    	
    	
    
    	if(parentId.indexOf("foldercategory-")>=0)
    	{
    		var j = parentId.indexOf("foldercategory-");
    		parentId = parentId.substr(j + 15);
    	}
    	else if (parentId.indexOf("folder-")>=0){
    		var j = parentId.indexOf("folder-");
    		parentId = parentId.substr(j + 7);
    	}
    
    	 var url = '<?= SITE_HTTP_ROOT; ?>slovar/edit/id/' + itemId + '/';
    	
    	 window.location = url;
    	 
    }
	
}


function Edit(itemId) {
    InitButtons(itemId);
    if (itemId > 0) {
        EditAction('exist');
    }
}


function AddObject(id , id_rub)
{
	
	alert(id_rub);
	
}

function Move(itemId, toId) {
    if (itemId!=toId) {
        var url = '<?= SITE_HTTP_ROOT; ?>rubrics/move/id/' + itemId + '/parent_id/' + toId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        OpenPopupWindow(url, w, h, param);
    }
}

function Refresh(itemId, parentId, action, name) {
    if (action=='delete') {
    	
        location.reload(true);
        
        /*tree.deleteItem(itemId, false)
        var sel = tree.getItemIdByIndex(parentId, 0)
        if (sel==undefined) {
            tree.selectItem(parentId, true, false);
        } else {
            tree.selectItem(sel, true, false);
        }
        InitButtons(null);*/
    }

    if (action=='edit') {
        if (tree.getIndexById(itemId)==null) {
            tree.insertNewChild(parentId,itemId,name)
        }
        tree.setItemText(itemId, name);
    }

    if (tree.getSelectedItemId()!='') {
        InitButtons(tree.getSelectedItemId());
    } else {
        InitButtons(null);
    }
}

//-->
</script>

<script language="JavaScript">
<!--
tree=new dhtmlXTreeObject("department-tree","100%","100%","root");
tree.attachEvent("onClick", InitButtons);

<?
if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
    ?>
        tree.attachEvent("onDblClick", Edit);
        tree.attachEvent("onDrop", Move);
        tree.enableDragAndDrop(false);
        tree.enableImageDrag(true);
    <?
}
?>
tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(true);
tree.enableDragAndDropScrolling(false);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=slovar&view=rubric&io_id=<?=$id?>");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=slovar&view=rubric&io_id=<?=$id?>");

InitButtons(null);

function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('department-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("department-view-tree");');


//-->
</script>


<p> Для редактирования атрибутов информационного объекта выберите объект </p>

<p> Выбор осуществляется нажатием кнопки мыши</p>