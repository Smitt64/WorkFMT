-- Вызывается из FillTable.cmd
-- Новиков А. 25.08.2004

truncate table &&1;

declare
 e_no_fmt     exception;
 pragma exception_init (e_no_fmt, -20002);
 e_no_trigger exception;
 pragma exception_init (e_no_trigger, -4080);
begin
 rsb_ainc.disable_trigger('&&1');
exception
 when e_no_trigger then
   null;
 when e_no_fmt then
   null;
end;
/


--@&&2
$sqlldr &&3/&&4@&&5 control=&&1..dat errors=10000
@&&1._blob


declare
 e_no_fmt     exception;
 pragma exception_init (e_no_fmt, -20002);
 e_no_trigger exception;
 pragma exception_init (e_no_trigger, -4080);
begin
  rsb_ainc.enable_trigger('&&1');
exception
 when e_no_trigger then
   null;
 when e_no_fmt then
   null;
end;
/

declare
 e_no_fmt     exception;
 pragma exception_init (e_no_fmt, -20002);
 e_no_trigger exception;
 pragma exception_init (e_no_trigger, -4080);
begin
  rsb_ainc.restore_seq('&&1');
exception
 when e_no_trigger then
   null;
 when e_no_fmt then
   null;
end;
/

exit
