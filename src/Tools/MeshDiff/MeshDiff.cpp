#include <boost/foreach.hpp>

#include "MeshDiff.hpp"

#include "Common/ComponentPredicates.hpp"

#include "Mesh/CRegion.hpp"
#include "Mesh/CElements.hpp"
#include "Mesh/CArray.hpp"
#include "Mesh/CTable.hpp"


using namespace CF;
using namespace CF::Common;
using namespace CF::Mesh;
using namespace Tools::Testing;

////////////////////////////////////////////////////////////////////////////////

namespace CF {
namespace Tools {
  
namespace Testing {

/// Compare 2D arrays
template<typename ValueT, typename ArrayT>
void array2d_test(const ArrayT a, const ArrayT b, Accumulator& result, const std::string& context)
{
  const Uint size_a = a.size();
  const Uint size_b = b.size();
  
  for(Uint i = 0, j = 0; i != size_a && j != size_b; ++i, ++j)
  {
    Accumulator acc;
    acc.max_ulps = result.max_ulps;
    vector_test(a[i], b[j], acc);
    const bool exact = boost::accumulators::min(acc.exact);
    const Real ulps = boost::accumulators::max(acc.ulps);
    if(!exact || ulps > acc.max_ulps)
    {
      CFerror << "In " << context << ", row " << i << ":\n";
      BOOST_FOREACH(const ValueT& val, a[i])
        CFerror << "  " << val;
      CFerror << "\n    differs from\n";
      BOOST_FOREACH(const ValueT& val, b[i])
        CFerror << "  " << val;
      CFerror << CFendl;
      if(exact)
        CFerror << "  ulps: " << ulps << CFendl;
    }
    result.exact(exact);
    result.ulps(ulps);
  }
  
  result.exact(size_a == size_b);
  
  if(size_a != size_b)
    CFerror << "Size difference in " << context << ": " << size_a << " != " << size_b << CFendl;
}

/// Compares CElements
void test(const CElements& a, const CElements& b, Accumulator& result)
{
  const CTable::ArrayT& table_a = a.connectivity_table().array();
  const CTable::ArrayT& table_b = b.connectivity_table().array();

  array2d_test<Uint>(table_a, table_b, result, "comparing " + a.full_path().string() + " and " + b.full_path().string());
}

/// Compares Arrays
void test(const CArray& a, const CArray& b, Accumulator& result)
{
  const CArray::ArrayT& array_a = a.array();
  const CArray::ArrayT& array_b = b.array();
  
  array2d_test<Real>(array_a, array_b, result, "comparing " + a.full_path().string() + " and " + b.full_path().string());
}
  
}
  
namespace MeshDiff {
  
////////////////////////////////////////////////////////////////////////////////

/// Compares two ranges
template<typename RangeT>
void compare_ranges(const RangeT& a, const RangeT& b, Accumulator& accumulator)
{
  range_test(a.begin(), a.end(), b.begin(), b.end(), accumulator);
}

bool diff(const Mesh::CMesh& a, const Mesh::CMesh& b, const Uint max_ulps)
{
  Accumulator accumulator;
  accumulator.max_ulps = max_ulps;
  // Compare Array data
  compare_ranges(recursive_range_typed<CArray>(a), recursive_range_typed<CArray>(b), accumulator);
  // Compare connectivity
  compare_ranges(recursive_range_typed<CElements>(a), recursive_range_typed<CElements>(b), accumulator);
  
  return boost::accumulators::min(accumulator.exact) && (boost::accumulators::max(accumulator.ulps) < max_ulps);
}

////////////////////////////////////////////////////////////////////////////////

} // MeshDiff
} // Tools
} // CF

////////////////////////////////////////////////////////////////////////////////
