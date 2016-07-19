import pymysql
import sys
from datetime import datetime, date, time, timedelta

dt = sys.argv[1]


conn= pymysql.connect(host="10.200.97.102", port = 3306, user="root", passwd = "", db = "test1")
cur = conn.cursor() 

cur.execute("select event_id, date, time from events where date=%s" % dt)

events = []

for rec in cur:
    event_id = rec[0]
    event_time = datetime.combine(rec[1], time(0)) + rec[2]
    ee = {}
    ee['event_id'] = event_id
    ee['event_time'] = event_time
    ee['components'] = []
    events.append(ee) 

for event in events:
    sqlstr ='select bloomberg_datum_id, datum_type, is_critical from component_mapping where release_id in(select release_id from event_release_mapping where event_id=\'%s\')' % event['event_id']
    print sqlstr 
    cur.execute(sqlstr)
    for rec in cur:
        comp = {}
        comp['bb_id'] = rec[0] + ' Index' 
        comp['bb_type'] = rec[1] 
        comp['is_critical'] = rec[2] 
        event['components'].append(comp)

cur.close()
conn.close()

for event in events:
    print '  eco_event {'
    print '    time: \"%s%s\"' % (event['event_time'].strftime('%H:%M:%S'), '.000')
    print '    eco_event_id: \"%s\"' % event['event_id'] 

    for comp in event['components']:
        print '    eco_component {'
        print '      bb_name: \"%s\"' % comp['bb_id'] 
        
        ss = 'LAST_PRICE' if (comp['bb_type'].count('ACTUAL') > 0) else 'ASK'
        print '      bb_field: \"%s\"' % ss 

        ss =  'false' if (comp['is_critical'] == '\x00') else 'true'
        print '      critical: %s' % ss
        print '    }'
    print '  }'
  
