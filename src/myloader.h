/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

        Authors:    David Ducos, Percona (david dot ducos at percona dot com)
*/

#include <mysql.h>

#ifndef _src_myloader_h
#define _src_myloader_h

#define AFTER_IMPORT_PER_TABLE "AFTER_IMPORT_PER_TABLE"
#define AFTER_IMPORT_ALL_TABLES "AFTER_IMPORT_ALL_TABLES"

struct thread_data {
  struct configuration *conf;
  MYSQL *thrconn;
  gchar *current_database;
  guint thread_id;
};

struct configuration {
  GAsyncQueue *database_queue;
  GAsyncQueue *table_queue;
  GAsyncQueue *data_queue;
  GAsyncQueue *post_table_queue;
  GAsyncQueue *view_queue;
  GAsyncQueue *post_queue;
  GAsyncQueue *ready;
  GAsyncQueue *pause_resume;
  GAsyncQueue *stream_queue;
  GList *table_list;
  GMutex * table_list_mutex;
  GHashTable *table_hash;
  GMutex *table_hash_mutex;
//  GList *schema_create_list;
  GList *checksum_list;
  GList *metadata_list;
  GMutex *mutex;
  GAsyncQueue *index_queue;
  int done;
};

struct database {
  gchar *name;
  gboolean schema_created;
};

struct db_table {
  char *database;
  char *real_database;
  char *table;
  char *real_table;
  guint64 rows;
  GAsyncQueue * queue;
  GList * restore_job_list;
  guint current_threads;
  guint max_threads;
  GMutex *mutex;
  GString *indexes;
  GString *constraints;
  guint count;
  gboolean schema_created;
  gboolean index_enqueued;
  GDateTime * start_data_time;
  GDateTime * finish_data_time;
  GDateTime * start_index_time;
  GDateTime * finish_time;
  gboolean completed;
  gint remaining_jobs;
};

enum file_type { INIT, SCHEMA_TABLESPACE, SCHEMA_CREATE, SCHEMA_TABLE, DATA, SCHEMA_VIEW, SCHEMA_TRIGGER, SCHEMA_POST, CHECKSUM, METADATA_TABLE, METADATA_GLOBAL, RESUME, IGNORED, LOAD_DATA, SHUTDOWN, INCOMPLETE };

#endif
