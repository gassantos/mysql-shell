table = session.get_schema('sakila').get_table('film_actor')

table.delete().where("actor_id=:ID").orderBy(['film_id desc']).limit(1).bind('ID',1).execute()

table.select().where('actor_id=:ID').orderBy(['film_id desc']).bind('ID',1).execute()