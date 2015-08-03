select createTempTables();
select setCurrentDl(4);

set client_encoding to utf8;

/*
insert into shu_acs(code, name, uri) values ('zarya22', 'zarya22', 'http://zaria22.acs.mil');
insert into shu_acs(code, name, uri) values ('vershina2', 'vershina2', 'http://vershina2.acs.mil');

insert into shu_domains(code, name) values('intelligence.Ground', 'intelligence.Ground');
insert into shu_domains(code, name) values('intelligence.Air', 'intelligence.Air');
insert into shu_domains(code, name) values('intelligence.Sea', 'intelligence.Sea');

insert into shu_positions(code, name) values('nachGRNapr', 'nachGRNapr');
insert into shu_positions(code, name) values('operDezh', 'operDezh');

insert into shu_orgs(code, name, uri) values('oskUVO', 'oskUVO', 'http://zaria22.acs.mil/ou/oskUVO');
insert into shu_orgs(code, name, uri) values('orgUnit', 'orgUnit', 'http://vershina2.acs.mil/ou/orgUnit');

insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('nachGRNapr', 'http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr', 1, 1, 1);
insert into shu_dls(name, uri, id_acs, id_org, id_pos) values('operDezh', 'http://vershina2.acs.mil/ou/orgUnit/pu/operDezh', 2, 2, 2);

*/

insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(1, '111', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>
<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://zaria22.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </with_uri>
			<with_post_id> nachGRNapr </with_post_id>
			<with_rank_id> army.pk </with_rank_id>
			<with_name> <![CDATA[ Смирнов П.А. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> cmd </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 577.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> HIGHEST </delivery_priority>
			<community_domain_membership> intelligence.Ground </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ 222 ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[ ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="4">
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>	
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="0">
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
		<human_readable_text> <![CDATA[ 
			Перевести систему в учебный режим
		]]></human_readable_text>
		
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(1, 1, current_timestamp, 1, 1);

/*
insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(2, '112', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://zaria22.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </with_uri>
			<with_post_id> nachGRNapr </with_post_id>
			<with_rank_id> army.pk </with_rank_id>
			<with_name> <![CDATA[ Смирнов П.А. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> ufdoc </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 577.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> LOWEST </delivery_priority>
			<community_domain_membership> intelligence.Ground </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[ Справка по моб. ресурсу ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="4">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>	
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="0">
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
		<human_readable_text> <![CDATA[
		СПРАВКА ПО МОБ. РЕСУРСУ
		
		Докладываю ...
		...
		что-то докладываю....
		...
		
		Докладчик: .....
		
		]]> </human_readable_text>
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(2, 2, current_timestamp, 1, 1);


insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(3, '113', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://zaria22.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </with_uri>
			<with_post_id> nachGRNapr </with_post_id>
			<with_rank_id> army.pk </with_rank_id>
			<with_name> <![CDATA[ Смирнов П.А. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> ufdoc </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 577.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> LOWEST </delivery_priority>
			<community_domain_membership> intelligence.Ground </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[ Боевое распоряжение ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="4">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>	
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="0">
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
		<human_readable_text> <![CDATA[
		БОЕВОЕ РАСПОРЯЖЕНИЕ
		
		Докладываю ...
		...
		что-то докладываю....
		...
		
		Докладчик: .....
		
		]]> </human_readable_text>
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(3, 3, current_timestamp, 1, 1);

insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(4, '114', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://vershina2.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </with_uri>
			<with_post_id> operDezh </with_post_id>
			<with_rank_id> army.ppk </with_rank_id>
			<with_name> <![CDATA[ Васечкин А.П. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> cmd </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 668.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> HIGHEST </delivery_priority>
			<community_domain_membership> intelligence.Ground </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ 111 ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[ ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="8">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorRIN.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="5">
				<has_type> post_unit </has_type>
				<has_uri> http://kriloA.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="6">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="7">
				<has_type> post_unit </has_type>
				<has_uri> http://more99.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="8">
				<has_type> post_unit </has_type>
				<has_uri> http://yarusM.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="1">
			<receipt number="1">
				<with_type> DR.1 </with_type>
				<for_each_addressee> false </for_each_addressee>
				<with_specific_data> <![CDATA[ ]]> </with_specific_data>
			</receipt>	
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
		<human_readable_text> <![CDATA[ 
			Привести войска в боевую готовность 
		]]></human_readable_text>
		
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(4, 4, current_timestamp, 1, 1);


insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(5, '115', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://vershina2.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </with_uri>
			<with_post_id> operDezh </with_post_id>
			<with_rank_id> army.ppk </with_rank_id>
			<with_name> <![CDATA[ Васечкин А.П. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> ufdoc </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 666.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> LOWEST </delivery_priority>
			<community_domain_membership> intelligence.Air </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[  Оперативная директива (по управлению ВВС) ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="8">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorRIN.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="5">
				<has_type> post_unit </has_type>
				<has_uri> http://kriloA.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="6">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="7">
				<has_type> post_unit </has_type>
				<has_uri> http://more99.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="8">
				<has_type> post_unit </has_type>
				<has_uri> http://yarusM.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="1">
			<receipt number="9">
				<with_type> DR.1 </with_type>
				<for_each_addressee> false </for_each_addressee>
				<with_specific_data> <![CDATA[ ]]> </with_specific_data>
			</receipt>	
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
		<human_readable_text> <![CDATA[ 
		ОПЕРАТ� ВНАЯ Д� РЕКТ� ВА (ПО УПРАВЛЕН� Ю ВВС)
		
		......
		Содержание директивы-сообщения......
		Продолжение содержания директивы-сообщения......
		......
		
		]]> </human_readable_text>
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(5, 5, current_timestamp, 1, 1);

insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(6, '116', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://vershina2.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </with_uri>
			<with_post_id> operDezh </with_post_id>
			<with_rank_id> army.ppk </with_rank_id>
			<with_name> <![CDATA[ Васечкин А.П. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> ufdoc </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 667.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> LOWEST </delivery_priority>
			<community_domain_membership> intelligence.Sea </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ ]]> </doc_name> 
				<doc_referat> <![CDATA[ ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[  Оперативная директива (по управлению ВМФ) ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="8">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorRIN.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="5">
				<has_type> post_unit </has_type>
				<has_uri> http://kriloA.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="6">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="7">
				<has_type> post_unit </has_type>
				<has_uri> http://more99.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="8">
				<has_type> post_unit </has_type>
				<has_uri> http://yarusM.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="1">
			<receipt number="9">
				<with_type> DR.1 </with_type>
				<for_each_addressee> false </for_each_addressee>
				<with_specific_data> <![CDATA[ ]]> </with_specific_data>
			</receipt>	
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
	</command_data>
	
	<formalized_document_data>
	</formalized_document_data> 
	
	<unformalized_document_data>
		<human_readable_text> <![CDATA[ 
		ОПЕРАТ� ВНАЯ Д� РЕКТ� ВА (ПО УПРАВЛЕН� Ю ВМФ)
		
		......
		Содержание директивы-сообщения......
		Продолжение содержания директивы-сообщения......
		......
		
		]]> </human_readable_text>
		<formalized_parameters>
			<parameters_description>
			</parameters_description>
			<parameters_values>
			</parameters_values>
		</formalized_parameters>
	</unformalized_document_data> 
	
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(6, 6, current_timestamp, 1, 1);


insert into jms_schema.message_data (id, uid, grif, priority, date_rec, type, body)
values(7, '117', 0, 0, current_timestamp, false, '<?xml version="1.0" encoding="UTF-8"?>

<msg>
<passport>
	<msg_created>
		<by_automated_control_system>
			<with_uri> http://vershina2.acs.mil </with_uri>
			<with_working_mode> 1 </with_working_mode>
			<on_address> <![CDATA[  ]]> </on_address>
		</by_automated_control_system>
		
		<by_organization_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit </with_uri>
		</by_organization_unit>
		
		<by_post_unit>
			<with_uri> http://vershina2.acs.mil/ou/orgUnit/pu/operDezh </with_uri>
			<with_post_id> operDezh </with_post_id>
			<with_rank_id> army.ppk </with_rank_id>
			<with_name> <![CDATA[ Васечкин А.П. ]]> </with_name>
		</by_post_unit>

		<as_having>
			<kind> fdoc </kind>
			<creation_date> 01.08.2011 </creation_date>
			<creation_time> 11:42:56.892 </creation_time>
			<sending_acs_registration_number> 667.2244.4754 </sending_acs_registration_number>
			<redirecting_acs_registration_number></redirecting_acs_registration_number>
			<mac_level> 0 </mac_level>
			<delivery_priority> LOWEST </delivery_priority>
			<community_domain_membership> intelligence.Sea </community_domain_membership>
			<command_specific_data_where>
				<cmd_code> <![CDATA[ ]]> </cmd_code>
			</command_specific_data_where>
			
			<formalized_document_specific_data_where>
				<doc_kind> </doc_kind> 
				<doc_name> <![CDATA[ formalized document]]> </doc_name> 
				<doc_referat> <![CDATA[ sdsdsd dsdsd sdsd ]]> </doc_referat>
			</formalized_document_specific_data_where>

			<unformalized_document_specific_data_where>
				<doc_name> <![CDATA[ ]]> </doc_name>
			</unformalized_document_specific_data_where>
			
			<attached_files_set with_files_amnt="0"> 
			</attached_files_set > 

			<response_msg_specific_data> <![CDATA[ ]]> </response_msg_specific_data>
			
			<parent_msg_which>
				<has_kind> </has_kind>
				<has_creation_date> </has_creation_date>
				<has_creation_time> </has_creation_time> 
				<has_sending_acs_registration_number> </has_sending_acs_registration_number>
				<has_redirecting_acs_registration_number> </has_redirecting_acs_registration_number>
				<has_receiving_acs_registration_number> </has_receiving_acs_registration_number>
				<has_response_msg_specific_data> </has_response_msg_specific_data>
			</parent_msg_which>
		</as_having>
		
		<to_be_delivered_to_addressees_where addressees_amount="8">
			<addressee number="1">
				<has_type> post_unit </has_type>
				<has_uri> http://zaria22.acs.mil/ou/oskUVO/pu/nachGRNapr </has_uri>
			</addressee>
			<addressee number="2">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorRIN.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="3">
				<has_type> post_unit </has_type>
				<has_uri> http://dozorSKVO.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="4">
				<has_type> post_unit </has_type>
				<has_uri> http://storm.acs.mil/ou/kom12GUMORF31600/pu/nachStFirstZam </has_uri>
			</addressee>
			<addressee number="5">
				<has_type> post_unit </has_type>
				<has_uri> http://kriloA.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="6">
				<has_type> post_unit </has_type>
				<has_uri> http://akacia.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="7">
				<has_type> post_unit </has_type>
				<has_uri> http://more99.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
			<addressee number="8">
				<has_type> post_unit </has_type>
				<has_uri> http://yarusM.acs.mil/ou/orgUnit/pu/operDezh </has_uri>
			</addressee>
		</to_be_delivered_to_addressees_where>
		
		<to_be_confirmed_about_delivering_by_receipts_where receipts_amount="1">
			<receipt number="9">
				<with_type> DR.1 </with_type>
				<for_each_addressee> false </for_each_addressee>
				<with_specific_data> <![CDATA[ ]]> </with_specific_data>
			</receipt>	
		</to_be_confirmed_about_delivering_by_receipts_where> 
	</msg_created>
</passport>

<body>
	<command_data>
	</command_data>
	<formalized_document_data> 
		<Information Echelon="String" CDomain="String">
			<EmergencySituation IncidentDateTime="2001-12-17T09:30:47Z" EmergencySituationType="String" Area="String" Region="String" MilitaryDistrict="String" Town="String">
				<RadiationPollutionDocument>
					<DocumentEntry>String</DocumentEntry> 
					<ObjectLongTitle>String</ObjectLongTitle>
					<SecretLongTitle>String</SecretLongTitle>
					<Belonging>
						<Bloc>String</Bloc>
						<Country>String</Country>
						<Department>String</Department>
						<Company>String</Company>
					</Belonging>
			
					<IncindentSituationalScheme>41394644363445313243</IncindentSituationalScheme>
			
					<RadiationPolution Authenticity="4096" InstrumentationEventTime="2001-12-17T09:30:47Z" CDomain="String" >
						<Partial>
							<IncindentShortDescription>String</IncindentShortDescription>
					
							<Coordinates HeightUnitOfMeasure="text" Height="3.14159265358979E0" Longitude="3.14159265358979E0" CDomain="String" Latitude="3.14159265358979E0" CoordinatesUnitOfMeasure="String"/><!--����������-->
					
							<LifeCycleState StateType="String">
								<Indicators>String</Indicators>
							</LifeCycleState>
					
							<DangerZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>
							<LimitedDangerZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>
							<SafeRangeZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>
							<RadiationLevelNextToSource UnitOfMeasurement="String" Value="3.14159265358979E0"/>
							<DangerZoneRadiationLevel UnitOfMeasurement="String" Value="3.14159265358979E0"/>
							<LimitedDangerZoneRadiationLevel UnitOfMeasurement="String" Value="3.14159265358979E0"/>
						</Partial>
					</RadiationPolution>
			
					<EmergencySituationDocumentAppendix>
						<EvaciuationActions>text</EvaciuationActions>
						<EmergencySituationLiquidationActions>String</EmergencySituationLiquidationActions>
					</EmergencySituationDocumentAppendix>
			
				</RadiationPollutionDocument>
			</EmergencySituation>
		</Information>
	</formalized_document_data> 
	<attached_files_set where_files_amnt="0">
	</attached_files_set> 
</body>
</msg>');

insert into jms_schema.in_mes (id, id_mes, date_get, from_obj, state)
values(7, 7, current_timestamp, 1, 1);
*/
set client_encoding to win;
