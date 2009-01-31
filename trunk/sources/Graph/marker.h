/**
 * File: marker.h - Interface and implementation of marker functionality.
 * Graph library, internal representation of graphs in ShowGraph tool.
 * Copyright (C) 2009  Boris Shurygin
 */

typedef unsigned short int MarkerIndex;
typedef unsigned int MarkerValue;

/**
 * Possible marker errors
 */
typedef enum MarkerErrorType_e
{
    /** Some error occured */
    M_ERROR_GENERIC,
    /** We've ran out of indexes. Someone forgot to free markers? */
    M_ERROR_OUT_OF_INDEXES,
    /** We're out of values. Seems to be interanl error of marker class.*/
    M_ERROR_OUT_OF_VALUES,
    /** Number of error types */
    M_ERROR_NUM
} MarkerErrorType;

/**
 * Marker description
 */
class Marker
{
    /** Markers index */
    MarkerIndex index;
    /** Value */
    MarkerValue value;

    /** Two classes have acces to marker internals. All others do not. */
    friend class Marked;
    friend class MarkerManager;
};

/**
 * Marker-related constants
 */
const short int MAX_GRAPH_MARKERS = 10; /** How many markers are allowed simultaneously */
const MarkerValue GRAPH_MARKER_CLEAN = 0;
const MarkerValue GRAPH_MARKER_FIRST = 1;
const MarkerValue GRAPH_MARKER_LAST = ( MarkerValue)( (int)-1);

/**
 * Represents a marked object
 */
class Marked
{
    /** Markers */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
public:
    /**
     * Mark node with marker. Return false if node is already marked. True otherwise.
     */
    inline bool Mark( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            return false;
        }
        markers[ marker.index] = marker.value;
        return true;
    }

    /**
     * Return true if node is marked with this marker
     */
    inline bool IsMarked( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            return true;
        }
        return false;
    }

    /**
     * Return true if node has been marked with this marker and unmarks it
     */
    inline bool Unmark( Marker marker)
    {
        if ( markers[ marker.index] == marker.value)
        {
            markers[ marker.index] = GRAPH_MARKER_CLEAN;
            return true;
        }
        return false;
    }
};


/**
 * Class that creates/frees markers
 */
class MarkerManager
{
    /**
     * Markers
     */
    MarkerValue markers[ MAX_GRAPH_MARKERS];
    bool is_used[ MAX_GRAPH_MARKERS];
    MarkerValue last;

    /** Implementatinal routines */
    /**
     * Find free index
     */
    inline MarkerIndex FindFreeIndex()
    {
        MarkerIndex i = 0;
        /** Search for free marker index */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            if ( !is_used [ i])
            {
                return i;
            }
        }
        throw M_ERROR_OUT_OF_INDEXES;
        return i;
    }
    
    /**
     * Increment marker value
     */
    inline MarkerValue GetNextValue()
    {
        if ( last == GRAPH_MARKER_LAST)
        {
            last = GRAPH_MARKER_FIRST;
        } else
        {
            last++;
        }
        return last;
    }
 
    /**
     * Check if this value is busy
     */
    inline bool IsValueBusy( MarkerValue val)
    {
        /** Check all markers */
        for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            if ( is_used [ i] && markers[ i] == val)
                return true;
        }
        return false;
    }

    /**
     * Return next free value
     */
    inline MarkerValue FindNextFreeValue()
    {
        MarkerIndex i = 0;
        bool reached_limit = false;
        MarkerValue res = last;
        
        while( IsValueBusy( res))
        {
            /** 
             * If we reached checked GRAPH_MARKER_LAST twice,
             * then we are in infinite loop and for some reason we don't free our markers
             */
            if ( res == GRAPH_MARKER_LAST)
            {
                Assert< MarkerErrorType> ( !reached_limit, 
                                           M_ERROR_OUT_OF_VALUES);
                reached_limit = true;            
            }
            res = GetNextValue();
        }
        return res;
    }

public:

    /**
     * Default Constructor
     */
    MarkerManager()
    {
        MarkerIndex i;

        /** Initialize markers */
        for ( i = 0; i < MAX_GRAPH_MARKERS; i++)
        {
            markers [ i] = GRAPH_MARKER_CLEAN;
            is_used [ i] = false;
        }
        last = GRAPH_MARKER_FIRST;
    }
    
    /**
     * Acquire new marker. Markers MUST be freed after use,
     * otherwise you run to markers number limit.
     */
    Marker NewMarker()
    {
        try {
            Marker new_marker;
                
            new_marker.index = FindFreeIndex();
            is_used[ new_marker.index] = true;
            new_marker.value = FindNextFreeValue();
            markers[ new_marker.index] = new_marker.value;
            return new_marker;
        } catch ( MarkerErrorType type)
        {
            /** Handle errors */
            switch ( type)
            {
                case M_ERROR_GENERIC:    
                default:
                    Assert(0);
            }
            Assert(0);
        }
        return NewMarker();
    }
    /**
     * Free marker
     */
    inline void FreeMarker( Marker m)
    {
        is_used[ m.index] = false;
    }
};

