
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifdef HAVE_MPI
#include "mpi.h"
#endif

#include "ref_mpi.h"

REF_INT ref_mpi_n = 1;
REF_INT ref_mpi_id = 0;

int ref_mpi_argc = 0;
char **ref_mpi_argv = NULL;

#ifdef HAVE_MPI

#define ref_type_mpi_type(macro_ref_type,macro_mpi_type)	\
  switch (macro_ref_type)					\
    {								\
    case REF_INT_TYPE: (macro_mpi_type) = MPI_INT; break;	\
    case REF_DBL_TYPE: (macro_mpi_type) = MPI_DOUBLE; break;	\
    default: RSS( REF_IMPLEMENT, "data type");			\
    }

#endif

static REF_DBL mpi_stopwatch_start_time;
static REF_DBL mpi_stopwatch_first_time;

REF_STATUS ref_mpi_start( int argc, char *argv[] )
{

  ref_mpi_argc = argc;
  ref_mpi_argv = argv;

#ifdef HAVE_MPI
  MPI_Init(&argc,&argv);

  MPI_Comm_size(MPI_COMM_WORLD,&ref_mpi_n);
  MPI_Comm_rank(MPI_COMM_WORLD,&ref_mpi_id);

  MPI_Barrier( MPI_COMM_WORLD ); 
  mpi_stopwatch_first_time = (REF_DBL)MPI_Wtime();
#else
  ref_mpi_n = 1;
  ref_mpi_id = 0;

  mpi_stopwatch_first_time = (REF_DBL)clock(  )/((REF_DBL)CLOCKS_PER_SEC);
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_stop( )
{

#ifdef HAVE_MPI
  MPI_Finalize( );
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_stopwatch_start( void )
{
#ifdef HAVE_MPI
  MPI_Barrier( MPI_COMM_WORLD ); 
  mpi_stopwatch_start_time = (REF_DBL)MPI_Wtime();
#else
  mpi_stopwatch_start_time = (REF_DBL)clock(  )/((REF_DBL)CLOCKS_PER_SEC);
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_stopwatch_stop( char *message )
{

#ifdef HAVE_MPI
  REF_DBL before_barrier, after_barrier, elapsed;
  REF_DBL first, last;
  before_barrier = (REF_DBL)MPI_Wtime()-mpi_stopwatch_start_time;
  MPI_Barrier( MPI_COMM_WORLD ); 
  after_barrier = (REF_DBL)MPI_Wtime();
  elapsed = after_barrier - mpi_stopwatch_first_time;
  after_barrier = after_barrier-mpi_stopwatch_start_time;
  RSS( ref_mpi_min( &before_barrier, &first, REF_DBL_TYPE), "min");
  RSS( ref_mpi_min( &after_barrier, &last, REF_DBL_TYPE), "max");
  if ( ref_mpi_master )
    printf("%9.4f: %16.12f (%16.12f) %6.2f%% load balance %s\n",
	   elapsed,
	   last,
	   first,
	   first/last*100.0,
	   message );
  RSS( ref_mpi_stopwatch_start(), "restart" );
#else
  printf("%9.4f: %16.12f (%16.12f) %6.2f%% load balance %s\n",
	 (REF_DBL)clock(  )/((REF_DBL)CLOCKS_PER_SEC) - 
	 mpi_stopwatch_first_time,
	 (REF_DBL)clock(  )/((REF_DBL)CLOCKS_PER_SEC) - 
	 mpi_stopwatch_start_time,
	 (REF_DBL)clock(  )/((REF_DBL)CLOCKS_PER_SEC) - 
	 mpi_stopwatch_start_time,
	 110.0,
	 message );
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_bcast( void *data, REF_INT n, REF_TYPE type )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;

  ref_type_mpi_type(type,datatype);

  MPI_Bcast(data, n, datatype, 0, MPI_COMM_WORLD);
#else
  SUPRESS_UNUSED_COMPILER_WARNING(data);
  SUPRESS_UNUSED_COMPILER_WARNING(n);
  SUPRESS_UNUSED_COMPILER_WARNING(type);
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_send( void *data, REF_INT n, REF_TYPE type, REF_INT dest )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;
  REF_INT tag;

  ref_type_mpi_type(type,datatype);

  tag = ref_mpi_n*dest+ref_mpi_id;

  MPI_Send(data, n, datatype, dest, tag, MPI_COMM_WORLD);
#else
  SUPRESS_UNUSED_COMPILER_WARNING(data);
  SUPRESS_UNUSED_COMPILER_WARNING(n);
  SUPRESS_UNUSED_COMPILER_WARNING(type);
  SUPRESS_UNUSED_COMPILER_WARNING(dest);
  return REF_IMPLEMENT;
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_recv( void *data, REF_INT n, REF_TYPE type, REF_INT source )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;
  REF_INT tag;
  MPI_Status status;

  ref_type_mpi_type(type,datatype);

  tag = ref_mpi_n*ref_mpi_id+source;

  MPI_Recv(data, n, datatype, source, tag, MPI_COMM_WORLD, &status);
#else
  SUPRESS_UNUSED_COMPILER_WARNING(data);
  SUPRESS_UNUSED_COMPILER_WARNING(n);
  SUPRESS_UNUSED_COMPILER_WARNING(type);
  SUPRESS_UNUSED_COMPILER_WARNING(source);
  return REF_IMPLEMENT;
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_alltoall( void *send, void *recv, REF_TYPE type )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;

  ref_type_mpi_type(type,datatype);

  MPI_Alltoall(send, 1, datatype, 
	       recv, 1, datatype, 
	       MPI_COMM_WORLD );
#else
  SUPRESS_UNUSED_COMPILER_WARNING(send);
  SUPRESS_UNUSED_COMPILER_WARNING(recv);
  SUPRESS_UNUSED_COMPILER_WARNING(type);
  return REF_IMPLEMENT;
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_alltoallv( void *send, REF_INT *send_size, 
			      void *recv, REF_INT *recv_size, 
			      REF_INT n, REF_TYPE type )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;
  REF_INT *send_disp;
  REF_INT *recv_disp;
  REF_INT *send_size_n;
  REF_INT *recv_size_n;
  REF_INT part;

  ref_type_mpi_type(type,datatype);

  send_size_n =(REF_INT *)malloc(ref_mpi_n*sizeof(REF_INT));
  RNS(send_size_n,"malloc failed");
  for ( part = 0; part<ref_mpi_n ; part++ )
    send_size_n[part] = n*send_size[part];

  recv_size_n =(REF_INT *)malloc(ref_mpi_n*sizeof(REF_INT));
  RNS(recv_size_n,"malloc failed");
  for ( part = 0; part<ref_mpi_n ; part++ )
    recv_size_n[part] = n*recv_size[part];

  send_disp =(REF_INT *)malloc(ref_mpi_n*sizeof(REF_INT));
  RNS(send_disp,"malloc failed");
  send_disp[0] = 0;
  for ( part = 1; part<ref_mpi_n ; part++ )
    send_disp[part] = send_disp[part-1]+send_size_n[part-1];

  recv_disp =(REF_INT *)malloc(ref_mpi_n*sizeof(REF_INT));
  RNS(recv_disp,"malloc failed");
  recv_disp[0] = 0;
  for ( part = 1; part<ref_mpi_n ; part++ )
    recv_disp[part] = recv_disp[part-1]+recv_size_n[part-1];

  MPI_Alltoallv(send, send_size_n, send_disp, datatype, 
		recv, recv_size_n, recv_disp, datatype, 
		MPI_COMM_WORLD );

  free(recv_disp);
  free(send_disp);
  free(recv_size_n);
  free(send_size_n);

#else
  SUPRESS_UNUSED_COMPILER_WARNING(send);
  SUPRESS_UNUSED_COMPILER_WARNING(send_size);
  SUPRESS_UNUSED_COMPILER_WARNING(recv);
  SUPRESS_UNUSED_COMPILER_WARNING(recv_size);
  SUPRESS_UNUSED_COMPILER_WARNING(n);
  SUPRESS_UNUSED_COMPILER_WARNING(type);
  return REF_IMPLEMENT;
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_min( void *input, void *output, REF_TYPE type )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;

  ref_type_mpi_type(type,datatype);

  MPI_Reduce( input, output, 1, datatype, MPI_MIN, 0, MPI_COMM_WORLD);

#else
  switch (type)
    {
    case REF_INT_TYPE: *(REF_INT *)input = *(REF_INT *)output; break;
    case REF_DBL_TYPE: *(REF_DBL *)input = *(REF_DBL *)output; break;
    default: RSS( REF_IMPLEMENT, "data type");
    }
  SUPRESS_UNUSED_COMPILER_WARNING(type);
#endif

  return REF_SUCCESS;
}

REF_STATUS ref_mpi_max( void *input, void *output, REF_TYPE type )
{
#ifdef HAVE_MPI
  MPI_Datatype datatype;

  ref_type_mpi_type(type,datatype);

  MPI_Reduce( input, output, 1, datatype, MPI_MAX, 0, MPI_COMM_WORLD);

#else
  switch (type)
    {
    case REF_INT_TYPE: *(REF_INT *)input = *(REF_INT *)output; break;
    case REF_DBL_TYPE: *(REF_DBL *)input = *(REF_DBL *)output; break;
    default: RSS( REF_IMPLEMENT, "data type");
    }
  SUPRESS_UNUSED_COMPILER_WARNING(type);
#endif

  return REF_SUCCESS;
}

