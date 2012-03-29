
--
-- PostgreSQL database dump
--
BEGIN;

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = error;
SET escape_string_warning = off;

--
-- Name: jms_schema; Type: SCHEMA; Schema: -; Owner: postgres
--

CREATE SCHEMA jms_schema;


ALTER SCHEMA jms_schema OWNER TO postgres;

--
-- Name: SCHEMA jms_schema; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA jms_schema IS 'Схема хранения данных обмена по JMS';


SET search_path = jms_schema, pg_catalog, public;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: in_mes; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE in_mes (
    id bigint NOT NULL,
    id_mes bigint,
    date_get timestamp without time zone,
    from_obj bigint,
    state integer DEFAULT 0
);
select setMacToNULL('in_mes');
--ALTER TABLE in_mes SET MAC TO '{0,0}';


ALTER TABLE jms_schema.in_mes OWNER TO postgres;

--
-- Name: TABLE in_mes; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE in_mes IS 'Данные входящих сообщений';


--
-- Name: COLUMN in_mes.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN in_mes.id IS 'Идентификатор';


--
-- Name: COLUMN in_mes.id_mes; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN in_mes.id_mes IS 'Данные ообщение (ссылка на таблицу с данными сообщения)';


--
-- Name: COLUMN in_mes.date_get; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN in_mes.date_get IS 'Дата получения сообщения';


--
-- Name: COLUMN in_mes.from_obj; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN in_mes.from_obj IS 'Откуда поступило сообщение (ссылка на таблицу объектов и URI)';


--
-- Name: COLUMN in_mes.state; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN in_mes.state IS 'Стадия обработки входящего сообщения: 0 - принято, но не обрабатывалось, 1 - принято, находится в обработке,
2 - обработано, отправлена квитанция о получении';


--
-- Name: kvit_in_match; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE kvit_in_match (
    id bigint NOT NULL,
    id_in_mes_kvit bigint,
    id_out_mes_match bigint
);
--ALTER TABLE kvit_in_match SET MAC TO '{0,0}';
select setMacToNULL('kvit_in_match');


ALTER TABLE jms_schema.kvit_in_match OWNER TO postgres;

--
-- Name: TABLE kvit_in_match; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE kvit_in_match IS 'Таблица соответствий входящих квитанций и исходящих сообщений';


--
-- Name: COLUMN kvit_in_match.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_in_match.id IS 'Идентификатор';


--
-- Name: COLUMN kvit_in_match.id_in_mes_kvit; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_in_match.id_in_mes_kvit IS 'Идентификатор входящей квитанции';


--
-- Name: COLUMN kvit_in_match.id_out_mes_match; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_in_match.id_out_mes_match IS 'Идентификатор исходящего сообщения';


--
-- Name: kvit_out_match; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE kvit_out_match (
    id bigint NOT NULL,
    id_out_mes_kvit bigint,
    id_in_mes_match bigint
);
--ALTER TABLE kvit_out_match SET MAC TO '{0,0}';
select setMacToNULL('kvit_out_match');


ALTER TABLE jms_schema.kvit_out_match OWNER TO postgres;

--
-- Name: TABLE kvit_out_match; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE kvit_out_match IS 'Таблица соответствий исходящих квитанций и входящих сообщений';


--
-- Name: COLUMN kvit_out_match.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_out_match.id IS 'Идентификатор';


--
-- Name: COLUMN kvit_out_match.id_out_mes_kvit; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_out_match.id_out_mes_kvit IS 'Идентификатор исходящей квитанции';


--
-- Name: COLUMN kvit_out_match.id_in_mes_match; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN kvit_out_match.id_in_mes_match IS 'Идентификатор входящего сообщения';


--
-- Name: message_data; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE message_data (
    id bigint NOT NULL,
    uid character varying(50) NOT NULL,
    grif integer NOT NULL,
    priority integer NOT NULL,
    date_rec timestamp without time zone,
    body text NOT NULL,
    type boolean DEFAULT false
);
--ALTER TABLE message_data SET MAC TO '{0,0}';
select setMacToNULL('message_data');


ALTER TABLE jms_schema.message_data OWNER TO postgres;

--
-- Name: TABLE message_data; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE message_data IS 'Сводные данные сообщения (регистрационный номер, гриф, приоритет и т.д.)';


--
-- Name: COLUMN message_data.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.id IS 'Идентификатор';


--
-- Name: COLUMN message_data.uid; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.uid IS 'Регистрационный номер';


--
-- Name: COLUMN message_data.grif; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.grif IS 'Гриф секретности сообщения
Возможные значения: 0 - несекретно, 1 - ДСП, 2 - секретно, 3 - сов. секретно  ';


--
-- Name: COLUMN message_data.priority; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.priority IS 'Приоритет доставки сообщения
Возможные значения: 0 - обыкновенный, 1 - самолёт, 2 - ракета, 3 - воздух, 4 - монолит';


--
-- Name: COLUMN message_data.date_rec; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.date_rec IS 'Дата записи в БД';


--
-- Name: COLUMN message_data.body; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.body IS 'Тело сообщения';


--
-- Name: COLUMN message_data.type; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN message_data.type IS 'Признак являются ли данные данного сообщения данными квитанции (true - являются, false - не являются)';


--
-- Name: out_mes; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE out_mes (
    id bigint NOT NULL,
    id_mes bigint,
    date_send timestamp without time zone
);
--ALTER TABLE out_mes SET MAC TO '{0,0}';
select setMacToNULL('out_mes');


ALTER TABLE jms_schema.out_mes OWNER TO postgres;

--
-- Name: TABLE out_mes; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE out_mes IS 'Данные исходящих сообщений';


--
-- Name: COLUMN out_mes.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN out_mes.id IS 'Идентификатор';


--
-- Name: COLUMN out_mes.id_mes; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN out_mes.id_mes IS 'Данные ообщение (ссылка на таблицу с данными сообщения)';


--
-- Name: COLUMN out_mes.date_send; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN out_mes.date_send IS 'Дата отправки';


--
-- Name: uri_kl; Type: TABLE; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE TABLE uri_kl (
    id bigint NOT NULL,
    name character varying(50),
    uri character varying(50),
    rus_name character varying(50)
);
--ALTER TABLE uri_kl SET MAC TO '{0,0}';
select setMacToNULL('uri_kl');

ALTER TABLE jms_schema.uri_kl OWNER TO postgres;

--
-- Name: TABLE uri_kl; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON TABLE uri_kl IS 'Реестр URI взаимодействующих объектов';


--
-- Name: COLUMN uri_kl.id; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN uri_kl.id IS 'Идентификатор';


--
-- Name: COLUMN uri_kl.name; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN uri_kl.name IS 'Имя объекта';


--
-- Name: COLUMN uri_kl.uri; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON COLUMN uri_kl.uri IS 'URI объекта';


--
-- Name: view_in; Type: VIEW; Schema: jms_schema; Owner: postgres
--

CREATE VIEW view_in AS
    SELECT in_mes.id AS in_mes_id, message_data.grif, message_data.priority, message_data.body FROM (in_mes LEFT JOIN message_data ON ((in_mes.id_mes = message_data.id))) WHERE (in_mes.state = 0) ORDER BY in_mes.id;


ALTER TABLE jms_schema.view_in OWNER TO postgres;

--
-- Name: VIEW view_in; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON VIEW view_in IS 'Просмотр принятых необработанных сообщений';


--
-- Name: view_out; Type: VIEW; Schema: jms_schema; Owner: postgres
--

CREATE VIEW view_out AS
    SELECT out_mes.id AS out_mes_id, message_data.uid, message_data.grif, message_data.priority, message_data.body FROM (out_mes LEFT JOIN message_data ON ((out_mes.id_mes = message_data.id))) WHERE (out_mes.date_send IS NULL) ORDER BY out_mes.id;


ALTER TABLE jms_schema.view_out OWNER TO postgres;

--
-- Name: VIEW view_out; Type: COMMENT; Schema: jms_schema; Owner: postgres
--

COMMENT ON VIEW view_out IS 'Просмотр  всех неотправленных сообщений';

--
-- Name: pk_in_mes; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY in_mes
    ADD CONSTRAINT pk_in_mes PRIMARY KEY (id);


--
-- Name: pk_kvit_in; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY kvit_in_match
    ADD CONSTRAINT pk_kvit_in PRIMARY KEY (id);


--
-- Name: pk_kvit_out_match; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY kvit_out_match
    ADD CONSTRAINT pk_kvit_out_match PRIMARY KEY (id);


--
-- Name: pk_message_data; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY message_data
    ADD CONSTRAINT pk_message_data PRIMARY KEY (id);


--
-- Name: pk_out_mes; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY out_mes
    ADD CONSTRAINT pk_out_mes PRIMARY KEY (id);


--
-- Name: pk_uri_kl; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY uri_kl
    ADD CONSTRAINT pk_uri_kl PRIMARY KEY (id);


--
-- Name: uniq; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY kvit_out_match
    ADD CONSTRAINT uniq_kvit_out_match UNIQUE (id_out_mes_kvit, id_in_mes_match);

ALTER TABLE ONLY kvit_in_match
    ADD CONSTRAINT uniq_kvit_in_match UNIQUE (id_in_mes_kvit, id_out_mes_match);

--
-- Name: uq_message_data_uid; Type: CONSTRAINT; Schema: jms_schema; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY message_data
    ADD CONSTRAINT uq_message_data_uid UNIQUE (uid);


--
-- Name: fki_out_mes_fr_key; Type: INDEX; Schema: jms_schema; Owner: postgres; Tablespace: 
--

CREATE INDEX fki_out_mes_fr_key ON out_mes USING btree (id_mes);


--
-- Name: fk_in_mes_message_data; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY in_mes
    ADD CONSTRAINT fk_in_mes_message_data FOREIGN KEY (id_mes) REFERENCES message_data(id);


--
-- Name: fk_in_mes_uri_kl; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY in_mes
    ADD CONSTRAINT fk_in_mes_uri_kl FOREIGN KEY (from_obj) REFERENCES uri_kl(id);


--
-- Name: fk_kvit_in_in_mes; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY kvit_in_match
    ADD CONSTRAINT fk_kvit_in_in_mes FOREIGN KEY (id_in_mes_kvit) REFERENCES in_mes(id);


--
-- Name: fk_kvit_in_out_mes; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY kvit_in_match
    ADD CONSTRAINT fk_kvit_in_out_mes FOREIGN KEY (id_out_mes_match) REFERENCES out_mes(id);


--
-- Name: fk_kvit_out_match_in_mes; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY kvit_out_match
    ADD CONSTRAINT fk_kvit_out_match_in_mes FOREIGN KEY (id_in_mes_match) REFERENCES in_mes(id);


--
-- Name: fk_kvit_out_match_out_mes; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY kvit_out_match
    ADD CONSTRAINT fk_kvit_out_match_out_mes FOREIGN KEY (id_out_mes_kvit) REFERENCES out_mes(id);


--
-- Name: out_mes_fr_key; Type: FK CONSTRAINT; Schema: jms_schema; Owner: postgres
--

ALTER TABLE ONLY out_mes
    ADD CONSTRAINT out_mes_fr_key FOREIGN KEY (id_mes) REFERENCES message_data(id);


--
-- PostgreSQL database dump complete
--

COMMIT;