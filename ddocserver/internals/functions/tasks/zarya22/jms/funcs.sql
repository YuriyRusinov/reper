SET client_encoding = 'UTF-8';
SET client_min_messages = error;
SET search_path = jms_schema;

BEGIN;

CREATE FUNCTION add_in_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text, from_obj_mes character varying, date_get_mes character varying) RETURNS integer
    AS $$DECLARE

id_new_mes_data integer;
id_new_in_mes integer;
date_add      text; 
state_new integer;
uri_id integer;

BEGIN

SELECT INTO id_new_mes_data case when max(id) IS NULL then 1 else max(id) + 1 end
	    FROM jms_schema.message_data;

SELECT INTO id_new_in_mes case when max(id) IS NULL then 1 else max(id) + 1 end
	    FROM jms_schema.in_mes;

date_add = to_char (LOCALTIMESTAMP, 'YYYY-MM-DD HH24:MI:SS');

EXECUTE 'INSERT INTO jms_schema.message_data (id, uid, grif, priority, date_rec, body, type) VALUES'
        ||'('||id_new_mes_data||','||quote_literal(uid_mes)||','||grif_mes||','||priority_mes||','||quote_literal(date_add)
        ||','||quote_literal(body_mes)||','||quote_literal(type_mes)||')';

state_new = 0;

EXECUTE 'INSERT INTO jms_schema.in_mes (id, id_mes, date_get, from_obj, state) VALUES'
        ||'('||id_new_in_mes||','||id_new_mes_data||','||quote_literal(to_timestamp(date_get_mes, 'DD.MM.YYYY HH24:MI:SS'))||','||jms_schema.get_uri_by_name(from_obj_mes)||','||state_new||')';    

	RETURN 1;

END$$
    LANGUAGE plpgsql;


ALTER FUNCTION jms_schema.add_in_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text, from_obj_mes character varying, date_get_mes character varying) OWNER TO postgres;
COMMENT ON FUNCTION jms_schema.add_in_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text, from_obj_mes character varying, date_get_mes character varying) IS 'Добавление данных нового вхядящего сообщения';

--
-- Name: add_out_mes(character varying, integer, integer, boolean, text); Type: FUNCTION; Schema: jms_schema; Owner: postgres
--

CREATE FUNCTION add_out_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text) RETURNS integer
    AS $$DECLARE

id_new_mes_data integer;
id_new_out_mes integer;
date_add      text; 

BEGIN

SELECT INTO id_new_mes_data case when max(id) IS NULL then 1 else max(id) + 1 end
	    FROM jms_schema.message_data;

SELECT INTO id_new_out_mes case when max(id) IS NULL then 1 else max(id) + 1 end
	    FROM jms_schema.out_mes;

date_add = to_char (LOCALTIMESTAMP, 'YYYY-MM-DD HH24:MI:SS');

EXECUTE 'INSERT INTO jms_schema.message_data (id, uid, grif, priority, date_rec, body, type) VALUES'
        ||'('||id_new_mes_data||','||quote_literal(uid_mes)||','||grif_mes||','||priority_mes||','||quote_literal(date_add)
        ||','||quote_literal(body_mes)||','||quote_literal(type_mes)||')';

EXECUTE 'INSERT INTO jms_schema.out_mes (id, id_mes) VALUES'
        ||'('||id_new_out_mes||','||id_new_mes_data||')';    

	RETURN 1;
END$$
    LANGUAGE plpgsql;


ALTER FUNCTION jms_schema.add_out_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text) OWNER TO postgres;

--
-- Name: FUNCTION add_out_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text); Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON FUNCTION add_out_mes(uid_mes character varying, grif_mes integer, priority_mes integer, type_mes boolean, body_mes text) IS 'Добавление данных нового исходящего сообщения';

--
-- Name: get_uri_by_name(character varying); Type: FUNCTION; Schema: jms_schema; Owner: postgres
--

CREATE FUNCTION get_uri_by_name(from_obj_mes character varying) RETURNS integer
    AS $$DECLARE

rec RECORD;
id_uri integer;

BEGIN

	FOR rec IN EXECUTE 'SELECT id from jms_schema.uri_kl where uri = '||quote_literal(from_obj_mes) LOOP

		id_uri:=rec.id;

	END LOOP;


	RETURN id_uri;

END$$
    LANGUAGE plpgsql;


ALTER FUNCTION jms_schema.get_uri_by_name(from_obj_mes character varying) OWNER TO postgres;

--
-- Name: FUNCTION get_uri_by_name(from_obj_mes character varying); Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON FUNCTION get_uri_by_name(from_obj_mes character varying) IS 'Получение идентификатора URI из реестра объектов';


--
-- Name: upd_in_mes(bigint, integer); Type: FUNCTION; Schema: jms_schema; Owner: postgres
--

CREATE FUNCTION upd_in_mes(id_in_mes bigint, state integer) RETURNS integer
    AS $$DECLARE

id_new_mes_data integer;

BEGIN

EXECUTE 'UPDATE jms_schema.in_mes SET state = '||state||' WHERE id = '||id_in_mes; 

	RETURN 1;

END$$
    LANGUAGE plpgsql;


ALTER FUNCTION jms_schema.upd_in_mes(id_in_mes bigint, state integer) OWNER TO postgres;

--
-- Name: FUNCTION upd_in_mes(id_in_mes bigint, state integer); Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON FUNCTION upd_in_mes(id_in_mes bigint, state integer) IS 'Изменение стутуса обработки входящего сообщения';


--
-- Name: upd_out_mes(bigint); Type: FUNCTION; Schema: jms_schema; Owner: postgres
--

CREATE FUNCTION upd_out_mes(id_out_mes bigint) RETURNS integer
    AS $$DECLARE

id_new_mes_data integer;
date_upd      text; 

BEGIN

date_upd = to_char (LOCALTIMESTAMP, 'DD.MM.YYYY HH24:MI:SS');

EXECUTE 'UPDATE jms_schema.out_mes SET date_send = '||quote_literal(date_upd)||' WHERE id = '||id_out_mes; 

	RETURN 1;

END$$
    LANGUAGE plpgsql;


ALTER FUNCTION jms_schema.upd_out_mes(id_out_mes bigint) OWNER TO postgres;

--
-- Name: FUNCTION upd_out_mes(id_out_mes bigint); Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON FUNCTION upd_out_mes(id_out_mes bigint) IS 'Изменение даты отправки исходящего сообщения';

COMMIT;
