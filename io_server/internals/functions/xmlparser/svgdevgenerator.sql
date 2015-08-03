create or replace function svgDeviceGenerator (int4, float8, float8, int4, int4) returns xml as
$BODY$
declare
    id_device alias for $1;
    pic_width alias for $2;
    pic_height alias for $3;
    view_width alias for $4;
    view_height alias for $5;

    result xml;
    svg_str varchar;
    svg_title varchar;
    svg_doc xml;
    rdev record;
    rgeom record;
    str_point varchar;
    i int4;
    is_quad_to boolean;
begin

    svg_str := E'<?xml version="1.0" standalone="no"?>\n<svg ';

    if (pic_width is not null and  pic_height is not null) then
        svg_str := svg_str || 'width="' || pic_width || 'cm" height="' || pic_height || 'cm" ';
    end if;

    svg_str := svg_str || 'viewBox="0 0 ' || view_width || ' ' || view_height || '" version="1.1">';

    select name into svg_title from devices d where d.id=id_device;
    svg_str := svg_str || E'\n' || '    <title>' || svg_title || '</title>';

    for rdev in
        select d.id as id_dev, es.id as id_element_shape, es.is_closed from
            devices d inner join
            element_shapes es on (d.id=es.id_device and d.id=id_device) --inner join
            --shape_segments as sh on (sh.id_element_shape = es.id) inner join
            --segment_types as seg_type on (sh.id_segment_type=seg_type.id)
    loop
        --raise notice '% %', rdev.points, rdev.id_segment_type;
        svg_str := svg_str || E'\n' || '        <path d="';

        for rgeom in
            select sh.points as points, seg_type.id as id_segment_type, seg_type.name as segment_type_name from shape_segments as sh inner join segment_types as seg_type on (sh.id_element_shape = rdev.id_element_shape and sh.id_segment_type=seg_type.id)
        loop
            raise notice '% %', rgeom.points, rgeom.id_segment_type;
            if (rgeom.segment_type_name = 'moveTo' or rgeom.segment_type_name = 'lineTo') then
                --
                -- moveTo or lineTo
                --
                if (array_upper(rgeom.points, 1) = 0) then
                    return warning 'Empty segment';
                    continue;
                end if;

                str_point := trim (both '()' from rgeom.points[1]::varchar);

                if (rgeom.segment_type_name = 'moveTo') then
                    svg_str := svg_str || 'M' || str_point;
                else
                    svg_str := svg_str || 'L' || str_point;
                end if;

                is_quad_to := false;

            elsif (rgeom.segment_type_name = 'quadTo') then
                --
                -- quadTo (first point)
                --
                if (not is_quad_to) then
                    svg_str := svg_str || 'Q';
                end if;

                is_quad_to := not is_quad_to;
                str_point := trim (both '()' from rgeom.points[1]::varchar);
                svg_str := svg_str || str_point;
                /*for i in 1..2
                loop
                    str_point := trim (both '()' from rgeom.points[i]::varchar);
                    svg_str := svg_str || str_point;
                    if (i < 2) then
                        svg_str := svg_str || ' ';
                    end if;
                end loop;
                */
            elsif (rgeom.segment_type_name = 'cubicTo') then
                --
                -- cubicTo
                --
                if (array_upper(rgeom.points, 1) <= 2) then
                    raise WARNING 'Insuffcient data for cubic curve';
                    continue;
                end if;
                svg_str := svg_str || 'C';
                for i in 1..3
                loop
                    str_point := trim (both '()' from rgeom.points[i]::varchar);
                    svg_str := svg_str || str_point;
                    if (i < 2) then
                        svg_str := svg_str || ' ';
                    end if;
                end loop;
            end if;
            svg_str := svg_str || ' ';
        end loop;
        if (rdev.is_closed) then
            svg_str := svg_str || 'z" ';
        else
            svg_str := svg_str || '" ';
        end if;
        svg_str := svg_str || 'fill="none" stroke="black" stroke-width="2" />';
    end loop;

    svg_str := svg_str || E'\n</svg>';
    raise notice '%', svg_str;
    svg_doc := xmlparse (document (svg_str));
    --svg_doc := xmlconcat (svg_doc, '</svg>');
    result := svg_doc;--xmlparse (svg_doc);
    -- <!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
    --result := xmlconcat (result, '</svg>');

    return result;
end
$BODY$
language 'plpgsql';

create or replace function svgDeviceGenerator (int4) returns xml as
$BODY$
declare
    id_device alias for $1;
begin

    return svgDeviceGenerator (id_device, null::int4, null::int4, 800, 600);
end
$BODY$
language 'plpgsql';
