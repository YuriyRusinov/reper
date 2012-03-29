<link rel="STYLESHEET" type="text/css" href="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.css" />
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxcommon.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/dhtmlxtree.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_xw.js"></script>
<script type="text/javascript" src="<?= SITE_URI_ROOT; ?>js/dhtmlx/ext/dhtmlxtree_my.js"></script>

<div id="department-tree" class="tree"></div>
<br/>
<div id="employee-table"></div>

<script type="text/javascript" language="JavaScript">
<!--
var sortColumn='department';
var sortDirection='asc';
var showFrom='0';
var selectedDepartment=0;

function EditRows(id )
{
    <?
        if ($is_enabled_rows_conf) {
            ?>
			    var w = 550;
			    var h = 200;
			    var param = '';
			    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/rows_conf/id/' + id + '/callback/Refresh/';
    			OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function DetailAction(id){
    var w = 550;
    var h = 200;
    var param = '';
    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/detail/id/' + id + '/';
    OpenPopupWindow(url, w, h, param);
}


function EditAction(id)
{
    <?
        if ($is_enabled_employee_edit) {
            ?>
                var w = 550;
                var h = 200;
                var param = '';
                var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/edit/id/' + id + '/callback/Refresh/';
                OpenPopupWindow(url, w, h, param);
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function DeleteAction(id)
{
    <?
        if ($is_enabled_employee_delete) {
            ?>
                if (confirm('После проведения операции, восстановление будет невозможно. Вы действительно хотите продолжить?')) {
                    var w = 1;
                    var h = 1;
                    var param = '';
                    var url = '<?= SITE_HTTP_ROOT; ?><?= $STORAGE['module']; ?>/delete/id/' + id + '/callback/Refresh/';
                    OpenPopupWindow(url, w, h, param);
                }
            <?
        } else {
            ?>
                alert('У Вас нет прав на выполнение операции');
            <?
        }
    ?>
}

function Refresh()
{
   	var itemId = selectedDepartment;//tree.getSelectedItemId();
	if(itemId == '') itemId=0;
   	var url = '<?= SITE_URI_ROOT.$STORAGE['module'].'/table/'; ?>';
   	HideElement('employee-table');
	url = url + 'ajax-request/1/department/'+ itemId+'/from/'+showFrom+'/sort/'+sortColumn+'/sort_dir/'+sortDirection+'/';
	LoadPage(url, 'employee-table');
	ShowElement('employee-table');
}

function Aclick(a)
{
	var i1 = a.href.indexOf('/sort/');
	sortColumn=a.href.substring(i1+6);
	i1 = sortColumn.indexOf('/');
	sortColumn=sortColumn.substring(0,i1);
	
	var i1 = a.href.indexOf('/sort_dir/');
	sortDirection=a.href.substring(i1+10);
	i1 = sortDirection.indexOf('/');
	sortDirection=sortDirection.substring(0,i1);

	var i1 = a.href.indexOf('/from/');
	showFrom=a.href.substring(i1+6);
	i1 = showFrom.indexOf('/');
	showFrom=showFrom.substring(0,i1);
	
   	HideElement('employee-table');
   	var url = a.href + 'ajax-request/1/';
	LoadPage(url, 'employee-table');
	ShowElement('employee-table');

	tree.selectItem(selectedDepartment,false,false);
	tree.openItem(selectedDepartment);

	return false;
}

//---------------department-tree-----------------------

function SelectDepartment(itemId) {
	selectedDepartment=itemId; 
    showFrom='0';
	var url = '<?= SITE_URI_ROOT.$STORAGE['module'].'/table/';?>';
   	HideElement('employee-table');
    url = url+ 'ajax-request/1/department/' + itemId+'/from/'+showFrom+'/sort/'+sortColumn+'/sort_dir/'+sortDirection+'/' ;
	LoadPage(url, 'employee-table');
    ShowElement('employee-table');
    if(itemId>0) tree.openItem(selectedDepartment);
	tree.selectItem(selectedDepartment,false,false);
}

tree=new dhtmlXTreeObject("department-tree","100%","100%","root");
tree.attachEvent("onDblClick", SelectDepartment);
tree.enableDragAndDrop(false);
tree.enableImageDrag(false);
tree.setImagePath("<?= SITE_URI_ROOT; ?>js/dhtmlx/imgs/csh_vista/");
tree.enableCheckBoxes(false);
tree.enableTreeLines(true);
tree.enableHighlighting(false);
tree.enableActiveImages(true);
tree.enableDragAndDropScrolling(true);
tree.setXMLAutoLoading("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree");
tree.attachEvent("onXLE", RestoreTree);
tree.loadXML("<?= SITE_HTTP_ROOT; ?>directory.php?module=department&view=tree&id=root");


function RestoreTree(treeObj)
{
    treeObj.attachEvent("onXLE", null);
    treeObj.loadOpenStates('department-view-tree');
}
window.onunload = new Function('tree.saveOpenStates("department-view-tree");');
window.onload = new Function('SelectDepartment(0);');

//-->
</script>
