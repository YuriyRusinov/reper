<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css">
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script  src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="category-manipulation">
    <div id="category-toolbar" class="toolbar">
        <button id="add-category" onclick="EditAction('new')"><img src="<?= SITE_URI_ROOT; ?>img/iconNew.gif" title="Добавить новую категорию">Добавить</button>
        <button id="edit-category" onclick="EditAction('exist')"><img src="<?= SITE_URI_ROOT; ?>img/iconEdit.gif" title="Редактировать категорию">Редактировать</button>
        <button id="delete-category" onclick="DeleteAction()"><img src="<?= SITE_URI_ROOT; ?>img/iconDelete.gif" title="Удалить категорию">Удалить</button>
    </div>
    <div id="category-tree" class="tree"></div>
</div>

<script language="JavaScript">
<!--
function InitButtons(selected_node) {
    document.getElementById('add-category').disabled = true;
    document.getElementById('edit-category').disabled = true;
    document.getElementById('delete-category').disabled = true;

    if (selected_node==null) {
        return;
    }

    if (selected_node>=0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('add-category').disabled = false;
                <?
            }
        ?>
    }

    if (selected_node != 0) {
        <?
            if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
                ?>
                    document.getElementById('edit-category').disabled = false;
                <?
            }

            if (access_enable($STORAGE, $STORAGE['module'], 'delete')) {
                ?>
                    document.getElementById('delete-category').disabled = false;
                <?
            }
        ?>
    }
}

function EditAction(type) {
    var itemId = tree.getSelectedItemId();
    var parentId = tree.getParentId(itemId);

    if (type=='new') {
        itemId = 0;
        parentId = tree.getSelectedItemId();
    }

    
    if (itemId.indexOf("folder-")>=0){
    	var i = itemId.indexOf("folder-");
    	itemId = itemId.substr(i + 7);
    }
    
    if (parentId.indexOf("folder-")>=0){
    	var j = parentId.indexOf("folder-");
    	parentId = parentId.substr(j + 7);
    }
    
    
    
    
    var url = '<?= SITE_HTTP_ROOT; ?>category/edit/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
    var w = 500;
    var h = 200;
    var param = '';
    //OpenPopupWindow(url, w, h, param);
    if(parentId !== "root")
    {
     	window.location = url;
    }
}

function DeleteAction() {
    var itemId = tree.getSelectedItemId();

    if (tree.hasChildren(itemId)>0) {
        alert('Запрещается удалять ветки, содержащие вложения.');
        return;
    }

    if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
        var parentId = tree.getParentId(itemId);
        
         if (itemId.indexOf("folder-")>=0){
    	var i = itemId.indexOf("folder-");
    	itemId = itemId.substr(i + 7);
    }
    
    if (parentId.indexOf("folder-")>=0){
    	var j = parentId.indexOf("folder-");
    	parentId = parentId.substr(j + 7);
    }
        
        var url = '<?= SITE_HTTP_ROOT; ?>category/delete/id/' + itemId + '/parent_id/' + parentId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        //OpenPopupWindow(url, w, h, param);
        window.location = url;
    }
}

function Edit(itemId) {
    InitButtons(itemId);
    if (itemId > 0) {
        EditAction('exist');
    }
}

function Move(itemId, toId) {
    if (itemId!=toId) {
        var url = '<?= SITE_HTTP_ROOT; ?>category/move/id/' + itemId + '/parent_id/' + toId + '/callback/Refresh';
        var w = 1;
        var h = 1;
        var param = '';
        //OpenPopupWindow(url, w, h, param);
        window.location = url;
    }
}

function Refresh(itemId, parentId, action, name) {
    if (action=='delete') {
        tree.deleteItem(itemId, false)
        var sel = tree.getItemIdByIndex(parentId, 0)
        if (sel==undefined) {
            tree.selectItem(parentId, true, false);
        } else {
            tree.selectItem(sel, true, false);
        }
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
tree=new dhtmlXTreeObject("category-tree","100%","100%","root");
tree.attachEvent("onClick", InitButtons);

<?
if (access_enable($STORAGE, $STORAGE['module'], 'edit')) {
    ?>
        tree.attachEvent("onDblClick", Edit);
        tree.attachEvent("onDrop", Move);
        tree.enableDragAndDrop(true);
        tree.enableImageDrag(true);
    <?
}
?>
tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(true);
tree.enableDragAndDropScrolling(true);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=category&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=category&view=tree&id=root");

InitButtons(null);

function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('category-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("category-view-tree");');


//-->
</script>
