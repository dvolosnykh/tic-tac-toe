function markerImage(marker, Marker)
{
    switch (marker) {
    case Marker.Cross:  return "cross.svg"
    case Marker.Circle: return "circle.svg"
    default:            return ""
    }
}
