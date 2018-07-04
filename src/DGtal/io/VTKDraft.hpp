template <typename TValue, typename TDataStream>
inline auto dataStreamToValue

template <typename TValue, typename TDataStream>
inline TValue dataStreamToValue(TDataStream & dataStream)
{
  return dataStreamToValueImpl<T>(dataStream, int(0));
}

template <
  typename TDomain,
  typename TImage,
  typename TFunctor
>
struct DataSetReader
{
  TDomain  const& myDomain;
  TImage        & myImage;
  TFunctor const& myFunctor;

  
  template <typename TVTKType>
  void apply( TVTKType const& vtk_type )
    {
      DataStream<TVTKType> data_stream(myFileStream);

      for ( auto const& pt : myDomain )
        {
          typename TImage::Value value{};
          for ( Dimension i = 0; i < TVTKType::dim(); ++i )
            value[i] = myFunctor( data_stream.read() );
          //for ( Dimension i = TVTKType::dim(); i < value.size(); ++i )
          //  value[i] = 0;
          myImage.setValue(pt, value);
        }

    }
};

// Factory

template <typename TImage, typename TFunctor>
read( std::string const& aFieldName, TImage const& anImage, TFunctor const& aFunctor )
{
  auto const& field_info = getFieldInfo(aFieldName);

  if (field_info)
    {
      myInputStream.seek(field_info.pos);
      auto reader = make_DataSetReader(myDomain, anImage, aFunctor);
      dispatch(reader, field_);
    }
  else
    {
      throw ...
    }

}
