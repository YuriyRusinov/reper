<?php

class resource_acl extends Zend_Acl
{
    public function __construct()
    {
        $db = db_class::get_instance();

        $sql = 'SELECT id_module, module FROM '.TABLE_MODULE;
        $db->query($sql);
        $resources = $db->value;

        $sql = 'SELECT id_role, name FROM '.TABLE_ROLE;
        $db->query($sql);
        $roles = $db->value;

        $sql = 'SELECT module_action_id, auto_enabled_action_id FROM '.TABLE_MODULE_AUTO_ACTION;
        $db->query($sql);
        $auto_actions = $db->value;

        $enable_action = array();
        foreach($auto_actions as $a) {
            $enable_action[$a['module_action_id']][] = $a['auto_enabled_action_id'];
        }


        $res = array();
        foreach($resources as $r) {
            $resource = new Zend_Acl_Resource($r['module']);
            $this->add($resource);
            $res[$r['id_module']] = $resource;
        }

        foreach($roles as $r) {
            $role = new Zend_Acl_Role($r['name']);
            $this->addRole($role);
            $this->allow($role, 'news', 'view'); //Разрешение на просмотр объявлений всем
            
            $sql = '
                        SELECT id_module_action, action, module_id
                        FROM '.TABLE_MODULE_ACTION.'
                        RIGHT JOIN '.TABLE_ROLE_ACTION.' ON module_action_id=id_module_action
                        WHERE role_id = '.$r['id_role'].' AND public_flag=\'yes\'
                     ';
            $db->query($sql);
            $actions = $db->value;

            foreach($actions as $a) {
                $this->allow($role, $res[$a['module_id']], $a['action']);

                if (array_key_exists($a['id_module_action'], $enable_action) && count($enable_action[$a['id_module_action']])>0) {
                    $sql = '
                                SELECT action, module_id
                                FROM '.TABLE_MODULE_ACTION.'
                                WHERE id_module_action IN ('.join(',', $enable_action[$a['id_module_action']]).') AND public_flag=\'no\'
                            ';
                    $db->query($sql);
                    $auto_actions = $db->value;

                    foreach($auto_actions as $action) {
                        $this->allow($role, $res[$action['module_id']], $action['action']);
                    }
                }
            }
        }

        $this->allow('Администратор', null, null);
    }
}