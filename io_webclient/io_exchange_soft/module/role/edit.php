<?
$page_title = 'Редактирование роли';

$id = intval(get_request_variable('id', 0));
$parent_id = intval(get_request_variable('parent_id', 0));
$name = get_request_variable('name', '');
$save = get_request_variable('save', '');
$enabled_action = get_request_variable('enabled_action', array());
$enabled_role_variables = get_request_variable('enabled_role_variables', array());
$_REQUEST['callback'] = get_request_variable('callback', '');

$role_actions = array();
$role = new role();
$role->id_role = $id;
if ($id > 0) {
    if (!$role->Load()) {
        $role->id_role = 0;
    } else {
        $role_actions = $role->GetRoleActionsId();
        $role_actions = array_flip($role_actions);

        $role_variables = $role->GetRoleVariablesId();
        $role_variables = array_flip($role_variables);
    }
}




// Получаем все типы дополнительных полей и сами поля
$variable = new variable();
$variable_modules = $variable->GetModules();

reset($variable_modules);
$variables = array();
while(list($variable_module, $variable_module_name)=each($variable_modules)) {
    $variable_modules[$variable_module] = array();
    $variable_modules[$variable_module]['name'] = $variable_module_name;
    $variable_modules[$variable_module]['vars'] = array();

    $variables = new collection();
    $variables->Load(TABLE_VARIABLE, false, '"module"=\''.db_class::str2base($variable_module).'\'', '"name" ASC');
    $variable_modules[$variable_module]['vars'] = $variables->_collection;
}



// Получаем все модули и все операции, связанные с ними
$modules = new collection();
$modules->Load(TABLE_MODULE, false, '', '"name" ASC');
$modules = $modules->_collection;

$module_actions = array();
for ($i=0;$i<count($modules);$i++) {
    $module_action_collection = new collection();
    $module_action_collection->Load(TABLE_MODULE_ACTION, false, 'module_id='.$modules[$i]->id_module.' AND public_flag=\'yes\'', '"name" ASC');

    $module_actions[$modules[$i]->id_module] = $module_action_collection->_collection;

    unset($module_action_collection);
}




if ($save) {
    $role_actions = array();
    reset($enabled_action);
    while(list($key,$val)=each($enabled_action)) {
        if ($val==1) $role_actions[$key] = 1;
    }

    $role_variables = array();
    reset($enabled_role_variables);
    while(list($variable_type, $data) = each($enabled_role_variables)) {
        $role_variables = array_merge($role_variables, $data);
    }
    $role_variables = array_flip($role_variables);

    $role->name = $name;
    $errors = $role->IsValidData();
    if (count($errors)==0) {
        $db = db_class::get_instance();
        $db->begin();

        $role->Save();

        $role->DeleteRoleActions();

        reset($role_actions);
        $role_action = new role_action();
        $role_action->role_id = $role->id_role;
        while(list($key, )=each($role_actions)) {
            $role_action->module_action_id = $key;
            $role_action->Save();
        }


        $role->DeleteRoleVariables();

        reset($role_variables);
        $role_variable = new role_variable();
        $role_variable->role_id = $role->id_role;
        while(list($key, )=each($role_variables)) {
            $role_variable->variable_id = $key;
            $role_variable->Save();
        }

        $db->commit();
    }
}

include SITE_FILE_ROOT.'template/simple_header.php';
include SITE_FILE_ROOT.'template/'.$STORAGE['module'].'/'.$STORAGE['action'].'.php';
include SITE_FILE_ROOT.'template/simple_footer.php';
