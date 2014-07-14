/**
 *  History.h
 *
 *  Generic ring-buffer implementation
**/

#ifndef HISTORY_h
#define HISTORY_h 1

#include <cctype>
#include <stdexcept>
#include <cstdio>
#include <cstdarg>

template <typename T>
class History
{
	private:
	
		size_t	m_top;
		size_t	m_size;
		T*		m_ringbuffer;
	
	public:
	
		History( size_t sz )
			: m_top( 0 ), m_size( sz ),
			  m_ringbuffer( new T[ ( sz > 0 ) ? sz : 1 ] )
		{
			assert( sz > 0 );
		}
		
		History( const History<T>& original )
			: m_top( original.m_top ), m_size( original.m_size )
		{
			m_ringbuffer = new T[ m_size ];
			
			for ( int i = 0; i < m_size; ++i )
			{
				m_ringbuffer[ i ] = original.m_ringbuffer[ i ];
			}
		}
		
		~History( )
		{
			delete[] m_ringbuffer;
		}
		
		void
		record( const T& value )
		{
			m_ringbuffer[ m_top ] = value;
			
			if ( ++m_top == m_size )
				m_top = 0;
		}
		
		T&
		retrieve( size_t back )
		{
			if ( back >= m_size )
				throw std::range_error( "history is not that long" );
			
			size_t pos = m_top + ( back > m_top ? m_size : 0 ) - back;
			
			return m_ringbuffer[ pos ];
		}
		
		T&
		operator[]( size_t back )
		{
			return retrieve( back );
		}
		
		T&
		operator=( const History<T>& rhs )
		{
			if ( m_ringbuffer )
			{
				delete m_ringbuffer;
			}
			
			m_top = rhs.m_top;
			m_size = rhs.m_size;
			m_ringbuffer = new T[ m_size ];
			
			for ( int i = 0; i < m_size; ++i )
			{
				m_ringbuffer[ i ] = rhs.m_ringbuffer[ i ];
			}
			
			return *this;
		}
		
		size_t
		size( ) const
		{
			return m_size;
		}
		
		void
		rewind()
		{
			if ( m_top == 0 )
				m_top = m_size;
			
			--m_top;
		}
		
		void
		advance()
		{
			if ( m_top == m_size );
				m_top = -1;
			
			++m_top;
		}
};

#endif
