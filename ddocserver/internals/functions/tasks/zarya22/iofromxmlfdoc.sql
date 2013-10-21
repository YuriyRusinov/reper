CREATE OR REPLACE FUNCTION iofromxmlfdoc(character varying)
  RETURNS boolean AS
$BODY$

declare
    value alias for $1;

    query varchar;
    tdate 	 varchar;
    ttime	 varchar;
    treg 	 varchar;
    tmac 	 varchar;
    tbody 	 varchar;
    torg 	 varchar; 
    tpost	 varchar;
    trank 	 varchar;
    tpost_id	 varchar;
    tpostname	 varchar; 
    tcode 	 varchar;

-- formalized document attributes/values 

    techelon varchar;
    tdomain varchar;
    tincidentdate varchar;
    tsittype varchar;
    tarea varchar;
    tregion varchar;
    tmildist varchar;
    ttown varchar;
    tdocument varchar;
    totitle varchar;
    tstitle varchar;
    tbloc varchar;
    tcountry varchar;
    tdepartment varchar;
    tcompany varchar;
    tsitscheme varchar;
    tauth varchar;
    tins varchar;
    tcdomain varchar;
    tshortdes varchar;
    thunit varchar;
    theight varchar;
    tlong varchar;
    tlatitude varchar;
    tmeasure varchar;
    tcoordomain varchar;
    tlife varchar;
    tlifeind varchar;
    tdangerradius varchar;
    tdangervalue varchar;
    tlimitedradius varchar;
    tlimitedvalue varchar;
    tsaferadius varchar;
    tsafevalue varchar;
    tradiation varchar;
    tradiationvalue varchar;
    tdangerrad varchar;
    tlimdangerrad varchar;
    tlimdangervalue varchar;
    tappendixevac varchar;
    tappendixemer varchar;
    
    isMakeIO boolean;
    r RECORD;

    idCat int4;
    ioName varchar;
    idOrg int4;
    idObject int4;
    idAttrRes int4;

    idDlSender int4;
    idDlReceiver int4;
    msgBody varchar;
    addresseeURI varchar;
    idMessage int4;

    deliveryPriority varchar;
    msgCreationDate varchar;
    msgCreationTime varchar;
    msgSenderUID varchar;
    msgRedirectingNumber varchar;
    receiptNumbers xml[];
    receiptNumber varchar;
    cnt int4;
    i int4;
    receiptType varchar;

    receiptXML xml;
    xml2Text text;
    muid varchar;
    
    
begin

    isMakeIO := true;
    
create temp table XXX (id_attr int4, a_val varchar);
-- block for formalized document

--    <Information Echelon="String" CDomain="String">


      techelon := '/msg/body/formalized_document_data/Information/@Echelon'; 
      query :=  'SELECT (xpath('||quote_literal(techelon) ||', '||quote_literal(value) ||':: xml))[1] As body';
      execute query INTO techelon;
      insert into XXX (id_attr, a_val) values (258, techelon);   
   
	tdomain := '/msg/body/formalized_document_data/Information/@CDomain'; 
	query :=  'SELECT (xpath('||quote_literal(tdomain) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tdomain;
        insert into XXX (id_attr, a_val) values (259, tdomain);

--<EmergencySituation IncidentDateTime="2001-12-17T09:30:47Z" EmergencySituationType="String" Area="String" Region="String" MilitaryDistrict="String" Town="String">

  tincidentdate := '/msg/body/formalized_document_data/Information/EmergencySituation/@IncidentDateTime'; 
   query :=  'SELECT (xpath('||quote_literal(tincidentdate) ||', '||quote_literal(value) ||'::xml))[1] As body';
   execute query INTO tincidentdate;
    insert into XXX (id_attr, a_val) values (241, tincidentdate);
   
    tsittype := '/msg/body/formalized_document_data/Information/EmergencySituation/@EmergencySituationType'; 
    query :=  'SELECT (xpath('||quote_literal(tsittype) ||', '||quote_literal(value) ||'::xml))[1] As body';
    execute query INTO tsittype;
     insert into XXX (id_attr, a_val) values (240, tsittype);


     tarea := '/msg/body/formalized_document_data/Information/EmergencySituation/@Area'; 
    query :=  'SELECT (xpath('||quote_literal(tarea) ||', '||quote_literal(value) ||'::xml))[1] As body';
    execute query INTO tarea;
     insert into XXX (id_attr, a_val) values (246, tarea);

     tregion := '/msg/body/formalized_document_data/Information/EmergencySituation/@Region'; 
    query :=  'SELECT (xpath('||quote_literal(tregion) ||', '||quote_literal(value) ||'::xml))[1] As body';
    execute query INTO tregion;
     insert into XXX (id_attr, a_val) values (245, tregion);

     tmildist := '/msg/body/formalized_document_data/Information/EmergencySituation/@MilitaryDistrict'; 
    query :=  'SELECT (xpath('||quote_literal(tmildist) ||', '||quote_literal(value) ||'::xml))[1] As body';
    execute query INTO tmildist;
     insert into XXX (id_attr, a_val) values (244, tmildist);

	ttown := '/msg/body/formalized_document_data/Information/EmergencySituation/@Town'; 
	query :=  'SELECT (xpath('||quote_literal(ttown) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO ttown;
	insert into XXX (id_attr, a_val) values (260, ttown);


  tdocument := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/DocumentEntry/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tdocument) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tdocument;
insert into XXX (id_attr, a_val) values (261, tdocument);


   totitle := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/ObjectLongTitle/text()'; 
  query :=  'SELECT (xpath('||quote_literal(totitle) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO totitle;
   insert into XXX (id_attr, a_val) values (242, totitle);

  tstitle := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/SecretLongTitle/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tstitle) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tstitle;
 insert into XXX (id_attr, a_val) values (243, tstitle);


	tbloc := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/Belonging/Bloc/text()'; 
	query :=  'SELECT (xpath('||quote_literal(tbloc) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tbloc;
	insert into XXX (id_attr, a_val) values (262, tbloc);

 tcountry := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/Belonging/Country/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tcountry) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tcountry;
insert into XXX (id_attr, a_val) values (263, tcountry);

 tdepartment := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/Belonging/Department/text()'; 
 query :=  'SELECT (xpath('||quote_literal(tdepartment) ||', '||quote_literal(value) ||':: xml))[1] As body';
 execute query INTO tdepartment;
 insert into XXX (id_attr, a_val) values (264, tdepartment);



 tcompany := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/Belonging/Company/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tcompany) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tcompany;
  insert into XXX (id_attr, a_val) values (265, tcompany);


--<IncindentSituationalScheme>41394644363445313243</IncindentSituationalScheme>
			
--<RadiationPolution Authenticity="4096" InstrumentationEventTime="2001-1217T09:30:47Z" CDomain="String" >

  tsitscheme := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/IncindentSituationScheme/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tsitscheme) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tsitscheme;
  insert into XXX (id_attr, a_val) values (257, tsitscheme);

  tauth := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/@Authenticity'; 
  query :=  'SELECT (xpath('||quote_literal(tauth) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tauth;
  insert into XXX (id_attr, a_val) values (267, tauth);

  tins := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/@InstrumentationEventTime'; 
  query :=  'SELECT (xpath('||quote_literal(tins) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tins;
  insert into XXX (id_attr, a_val) values (268, tins);


	tcdomain := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/@CDomain'; 
	query :=  'SELECT (xpath('||quote_literal(tcdomain) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tcdomain;
	insert into XXX (id_attr, a_val) values (270, tcdomain);


--         <Partial>
--	 <IncindentShortDescription>String</IncindentShortDescription>
--	<Coordinates HeightUnitOfMeasure="text" Height="3.14159265358979E0" Longitude="3.14159265358979E0" CDomain="String" Latitude="3.14159265358979E0" CoordinatesUnitOfMeasure="String"/>


tshortdes := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/IncindentShortDescription/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tshortdes) ||',  '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tshortdes;
   insert into XXX (id_attr, a_val) values (249, tshortdes);
  
  
  thunit := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@HeightUnitOfMeasure'; 
  query :=  'SELECT (xpath('||quote_literal(thunit) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO thunit;
insert into XXX (id_attr, a_val) values (271, thunit);


	theight := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@Height'; 
	query :=  'SELECT (xpath('||quote_literal(theight) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO theight;
insert into XXX (id_attr, a_val) values (272, theight);


tlong := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@Longitude'; 
  query :=  'SELECT (xpath('||quote_literal(tlong) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlong;
   insert into XXX (id_attr, a_val) values (248, tlong);

tlatitude := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@Latitude'; 
  query :=  'SELECT (xpath('||quote_literal(tlatitude) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlatitude;
   insert into XXX (id_attr, a_val) values (247, tlatitude);


	tmeasure := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@CoordinatesUnitOfMeasure'; 
	query :=  'SELECT (xpath('||quote_literal(tmeasure) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tmeasure;
insert into XXX (id_attr, a_val) values (273, tmeasure);

   tcoordomain := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/Coordinates/@CDomain'; 
  query :=  'SELECT (xpath('||quote_literal(tcoordomain) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tcoordomain;  
insert into XXX (id_attr, a_val) values (276, tcoordomain);


--			<LifeCycleState StateType="String">
--			    <Indicators>String</Indicators>
--			</LifeCycleState>
										
tlife := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LifeCycleState/@StateType'; 
  query :=  'SELECT (xpath('||quote_literal(tlife) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlife;  
insert into XXX (id_attr, a_val) values (274, tlife);

tlifeind := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LifeCycleState/Indicators/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tlifeind) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlifeind;
  insert into XXX (id_attr, a_val) values (275, tlifeind);

--		<DangerZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>

  tdangerradius := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/DangerZoneRadius/@UnitOfMeasurement'; 
  query :=  'SELECT (xpath('||quote_literal(tdangerradius) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tdangerradius;
  insert into XXX (id_attr, a_val) values (277, tdangerradius);
  

   tdangervalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/DangerZoneRadius/@Value'; 
   query :=  'SELECT (xpath('||quote_literal(tdangervalue) ||', '||quote_literal(value) ||'::xml))[1] As body';
   execute query INTO tdangervalue;
   insert into XXX (id_attr, a_val) values (251, tdangervalue);
  

--		<LimitedDangerZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>

  tlimitedradius := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LimitedDangerZoneRadius/@UnitOfMeasurement'; 
  query :=  'SELECT (xpath('||quote_literal(tlimitedradius) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tlimitedradius;
insert into XXX (id_attr, a_val) values (278, tlimitedradius);


  tlimitedvalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LimitedDangerZoneRadius/@Value'; 
  query :=  'SELECT (xpath('||quote_literal(tlimitedvalue) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlimitedvalue;
  insert into XXX (id_attr, a_val) values (253, tlimitedvalue);

--		<SafeRangeZoneRadius UnitOfMeasurement="String" Value="3.14159265358979E0"/>

  tsaferadius := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/SafeRangeZoneRadius/@UnitOfMeasurement'; 
  query :=  'SELECT (xpath('||quote_literal(tsaferadius) ||', '||quote_literal(value) ||':: xml))[1] As body';
  execute query INTO tsaferadius;
insert into XXX (id_attr, a_val) values (279, tsaferadius);

	tsafevalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/SafeRangeZoneRadius/@Value'; 
	query :=  'SELECT (xpath('||quote_literal(tsafevalue) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tsafevalue;
	insert into XXX (id_attr, a_val) values (283, tsafevalue);

--		<RadiationLevelNextToSource UnitOfMeasurement="String" Value="3.14159265358979E0"/>
	tradiation := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/RadiationLevelNextToSource/@UnitOfMeasurement'; 
	query :=  'SELECT (xpath('||quote_literal(tradiation) ||', '||quote_literal(value) ||' :: xml))[1] As body';
	execute query INTO tradiation;
	insert into XXX (id_attr, a_val) values (280, tradiation);

	tradiationvalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/RadiationLevelNextToSource/@Value'; 
	query :=  'SELECT (xpath('||quote_literal(tradiationvalue) ||', '||quote_literal(value) ||'::xml))[1] As body';
	execute query INTO tradiationvalue;
	insert into XXX (id_attr, a_val) values (250, tradiationvalue);
  
--		<DangerZoneRadiationLevel UnitOfMeasurement="String" Value="3.14159265358979E0"/>
	tdangerrad := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/DangerZoneRadiationLevel/@UnitOfMeasurement'; 
	query :=  'SELECT (xpath('||quote_literal(tdangerrad) ||', '||quote_literal(value) ||' :: xml))[1] As body';
	execute query INTO tdangerrad;
	insert into XXX (id_attr, a_val) values (281, tdangerrad);

	tdangervalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/DangerZoneRadiationLevel/@Value'; 
	query :=  'SELECT (xpath('||quote_literal(tdangervalue) ||', '||quote_literal(value) ||'::xml))[1] As body';
	execute query INTO tdangervalue;
	insert into XXX (id_attr, a_val) values (252, tdangervalue);
  
  
-- <LimitedDangerZoneRadiationLevel UnitOfMeasurement="String" Value="3.14159265358979E0"/>
-- </Partial>
-- </RadiationPolution>

	tlimdangerrad := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LimitedDangerZoneRadiationLevel/@UnitOfMeasurement'; 
	query :=  'SELECT (xpath('||quote_literal(tlimdangerrad) ||', '||quote_literal(value) ||':: xml))[1] As body';
	execute query INTO tlimdangerrad;
	--insert into XXX (id_attr, a_val) values (250, tdangervalue);
	insert into XXX (id_attr, a_val) values (282, tlimdangerrad);

tlimdangervalue := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/RadiationPolution/Partial/LimitedZoneRadiationLevel/@Value'; 
  query :=  'SELECT (xpath('||quote_literal(tlimdangervalue) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tlimdangervalue;
  insert into XXX (id_attr, a_val) values (254, tdangervalue);


-- <EmergencySituationDocumentAppendix>
-- <EvaciuationActions>text</EvaciuationActions>
-- <EmergencySituationLiquidationActions>String</EmergencySituationLiquidationActions

tappendixevac := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/EmergencySituationDocumentAppendix/EvaciuationActions/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tappendixevac) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tappendixevac;

   insert into XXX (id_attr, a_val) values (256, tappendixevac);
  

tappendixemer := '/msg/body/formalized_document_data/Information/EmergencySituation/RadiationPollutionDocument/EmergencySituationDocumentAppendix/EmergencySituationLiquidationActions/text()'; 
  query :=  'SELECT (xpath('||quote_literal(tappendixemer) ||', '||quote_literal(value) ||'::xml))[1] As body';
  execute query INTO tappendixemer;

   insert into XXX (id_attr, a_val) values (255, tappendixemer);
  


 isMakeIO := true;
    
    
-- get date creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_date/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As date';
    execute query INTO msgCreationDate;
    insert into XXX (id_attr, a_val) values (233, msgCreationDate);

-- get time creating message
    tbody :=  '/msg/passport/msg_created/as_having/creation_time/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As time';
    execute query INTO msgCreationTime;
    insert into XXX (id_attr, a_val) values (234, msgCreationTime);

-- get sending_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/sending_acs_registration_number/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||':: xml))[1] As reg';
    execute query INTO msgSenderUID;
    --insert into XXX (id_attr, a_val) values (233, tdate);

-- get redirecting_acs_registration_number
    tbody :=  '/msg/passport/msg_created/as_having/redirecting_acs_registration_number/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As reg';
    execute query INTO msgRedirectingNumber;

-- get mac level message
    tbody :=  '/msg/passport/msg_created/as_having/mac_level/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As mac_level';
    execute query INTO tmac;
    insert into XXX (id_attr, a_val) values (235, tmac);

    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_uri/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_acs';
    execute query INTO torg;
    query =  'select id from shu_acs where uri = trim(' || quote_literal(torg)||')';
    execute query into torg;
    insert into XXX (id_attr, a_val) values (225, torg);


    tbody :=  '/msg/passport/msg_created/by_automated_control_system/with_working_mode/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As mode';
    execute query INTO torg;
    
    insert into XXX (id_attr, a_val) values (231, torg);



-- get organization uri
    tbody :=  '/msg/passport/msg_created/by_organization_unit/with_uri/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_org';
    execute query INTO torg;
    query =  'select id from shu_orgs where uri = trim(' || quote_literal(torg)||')';
    execute query into torg;
    insert into XXX (id_attr, a_val) values (226, torg);

-- get post uri
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As uri_post';
    execute query INTO tpost;
    query =  'select id from shu_dls where uri = trim(' || quote_literal(tpost)||')';
    execute query into tpost;
    insert into XXX (id_attr, a_val) values (228, tpost);


-- get rank id
    tbody :=  '/msg/passport/msg_created/by_post_unit/with_rank_id/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As rank_id';
    execute query INTO trank;
    query =  'select id from ranks where code = trim(' || quote_literal(trank)||')';
    execute query into trank;
    insert into XXX (id_attr, a_val) values (20, trank);

-- get domain
    tbody :=  '/msg/passport/msg_created/as_having/community_domain_membership/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As domain';
    execute query INTO trank;
    query =  'select id from shu_domains where code = trim(' || quote_literal(trank)||')';
    execute query into trank;
    insert into XXX (id_attr, a_val) values (230, trank);


-- priority

     tbody :=  '/msg/passport/msg_created/as_having/delivery_priority/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As prior';
    execute query INTO deliveryPriority;
    insert into XXX (id_attr, a_val) values (236, deliveryPriority);


-- name and text 
    tbody :=  '/msg/passport/msg_created/as_having/formalized_document_specific_data_where/doc_name/text()';
    query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[2] ::varchar)';
    execute query INTO tcode;

   

    tbody :=  '/msg/passport/msg_created/by_post_unit/with_name/text()';
    query := 'SELECT trim((xpath('||quote_literal(tbody) ||',  ' || quote_literal(value) ||'::xml))[2]::varchar)';
    execute query INTO tpostname;
    insert into XXX (id_attr, a_val) values (25, tpostname);

    idCat = 170;

    if tcode is null
    then
        tcode := 'Входящий формализованный документ (Донесение о ЧС)';
    end if;

    ioName := tcode;

    select into idOrg id_organization from getlocalorg();

    select into idObject ioinsert (ioName, 
                                   idCat, 
                                   2/*jupiter*/, --author
                                   1, --id_io_state
                                   null::varchar, --table_name
                                   null::varchar, --desc
                                   NULL::varchar, --info
                                   1,  --id_maclabel
                                   NULL::varchar, --unique_id
                                   1, --id_sync_type
                                   idOrg, 
                                   false, --is_global
                                   null::int4, --id_search_template
                                   null::varchar, --ref_table
                                   null::int8, --fill
                                   null::int8, --text
                                   1, --id_type
                                   NULL::VARCHAR,--r_icon
                                   NULL::uuid);--uuid_t
    if (idObject is null or idObject <= 0) then
        return false;
    end if;
    

    for r in
        select id_attr, a_val from XXX
    loop
        select into idAttrRes ioInsertAttr (idObject, r.id_attr, quote_literal (r.a_val)::varchar, current_timestamp::timestamp without time zone, null::timestamp without time zone, null::int4, null::int4, null::varchar);
        if (idAttrRes is null or idAttrRes <0) then
            return null;
        end if;
    end loop;
 
    drop table XXX;


    tbody :=  '/msg/passport/msg_created/to_be_delivered_to_addressees_where/addressee[@number=1]/has_uri/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1] As addressee';
    execute query INTO addresseeURI;
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(addresseeURI) ||') and sd.id = sdp.id_shu_dls;';
    execute query into idDlReceiver;
    if(idDlReceiver isnull) then
        idDlReceiver = 4;  --admin
    end if;
    

    tbody :=  '/msg/passport/msg_created/by_post_unit/with_uri/text()';
    query := 'SELECT (xpath('||quote_literal(tbody) ||',  '||quote_literal(value) ||'::xml))[1] As uri_post';
    execute query INTO tpost;
    
    query =  'select sdp.id_position from shu_dls sd, shu_dls_position sdp where sd.uri = trim(' || quote_literal(tpost) ||') and sd.id = sdp.id_shu_dls;';
    execute query into idDlSender;
    if(idDlSender isnull) then
        idDlSender = 4;  --admin
    end if;


    if(idDlSender isnull or idDlReceiver isnull) then
        raise warning 'Cannot create message in message journal! % (%) --- % (%)', idDlSender, tpost, idDlReceiver, addresseeURI;
        if(getCurrentUser() <> 2) then --jupiter
            insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
            values(idObject, -10, true, false, false, false, false);
        end if;
        return false;
    end if;

    insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
    values(idObject, idDlSender, true, true, true, true, true);

    if(idDlSender <> idDlReceiver) then
        insert into access_table (id_io_object, id_role, allow_readlist, allow_read, allow_delete, allow_update, allow_send)
        values(idObject, idDlReceiver, true, true, true, true, true);
    end if;

    select getNextSeq('message_journal', 'id') into idMessage;

    msgBody = ioName;

    insert into message_journal(id, 
                                id_dl_sender, 
                                id_dl_receiver, 
                                id_io_object, 
                                insert_time, 
                                sent_datetime, 
                                receive_datetime, 
                                read_datetime, 
                                message_body,
                                id_urgency_level) 
    values
                                    
                               (idMessage, 
                                idDlSender, 
                                idDlReceiver,
                                idObject, 
                                current_timestamp, 
                                NULL, 
                                NULL,
                                NULL,
                                msgBody,
                                1);


    tbody := '/msg/passport/msg_created/to_be_confirmed_about_delivering_by_receipts_where/receipt/@number';
    query := 'SELECT (xpath('||quote_literal(tbody) ||', '|| quote_literal(value) ||'::xml)) As date';
    execute query INTO receiptNumbers;

    cnt = array_upper (receiptNumbers, 1);
    
    if(receiptNumbers isnull or cnt isnull or cnt <= 0) then
        return false;
    end if;
    for i in 1..cnt 
    loop
        receiptNumber = receiptNumbers[i]::varchar;
        tbody :=  '/msg/passport/msg_created/to_be_confirmed_about_delivering_by_receipts_where/receipt[@number=' || 
                  asString(receiptNumber, false) || 
                  ']/with_type/text()';
 
        query := 'SELECT trim((xpath('||quote_literal(tbody) ||', '||quote_literal(value) ||'::xml))[1]::varchar) As date';
        execute query INTO receiptType;
        if(lower(receiptType) <> 'dr.3') then
            continue;
        end if;

        select receiptToXML(receiptType, deliveryPriority, 'fdoc', msgCreationDate, msgCreationTime, msgSenderUID, addresseeURI, msgRedirectingNumber) into receiptXML;
        if(receiptXML isnull) then
            continue;
        end if;

        xml2Text = xmlText(receiptXML);

        select unique_id into muid from message_journal where id = idMessage;
        if(muid isnull) then
            continue;
        end if;
        
        perform jms_schema.add_out_mes (muid || '-receipt', 0, 0, true, xml2Text);
    end loop;    
    
    return isMakeIO;

end
$BODY$
LANGUAGE plpgsql;
