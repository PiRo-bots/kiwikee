const {circle, square} = require('@jscad/csg/api').primitives2d
const {cylinder, sphere, cube} = require('@jscad/csg/api').primitives3d
const {color} = require('@jscad/csg/api').color
const {hull} = require('@jscad/csg/api').transformations
const {rotate, translate, scale, mirror, contract, expand} = require('@jscad/csg/api').transformations
const {union, difference} = require('@jscad/csg/api').booleanOps

const {rectangle, ellipse} = require('../lib/V2mock/primitives2d')
const {linearExtrude} = require('../lib/V2mock/extrusions')

const trackSegment = params => {
  const trackData = {
    width: 16,
    length: 12,
    // holeOffset
    thicknes: 4,
    body: {
      size: [12, 16, 3.5]
    },
    innerNotch: {
      offset: 4,
      size: [5, 5, 4]
    },
    mountHole: {
      diameter: 2
    },
    wheelHole: {
      diameter: 7,
      segments: 4
    }
  }
  const {body, innerNotch, mountHole, wheelHole} = trackData

  const blockDiameter = body.size[2]
  const bodyLength = body.size[0]
  const holeOffset = bodyLength / 2 - blockDiameter / 2
  const sideBlocksWidth = (body.size[1] - innerNotch.size[1]) / 2

  console.log('sideBlocksWidth', sideBlocksWidth)

  let sideOutline = hull(
    translate([bodyLength / 2 - blockDiameter / 2, 0, 0], ellipse({r: blockDiameter / 2, center: true})),
    translate([-bodyLength / 2 + blockDiameter / 2, 0, 0], ellipse({r: blockDiameter / 2, center: true}))
  )
  sideOutline = difference(
    sideOutline,
    translate([holeOffset, 0, 0], ellipse({r: mountHole.diameter / 2, center: true}))
  )

  const sideShape = rotate([90, 0, 0], linearExtrude({height: sideBlocksWidth}, sideOutline))

  let trackBody = union(
    translate([0, 0, 0], sideShape),
    mirror([1, 0, 0], translate([innerNotch.offset, sideBlocksWidth, 0], sideShape)),
    translate([0, sideBlocksWidth + innerNotch.size[1], 0], sideShape)
  )
  trackBody = translate([0, -innerNotch.size[1] / 2, innerNotch.size[2] / 2], trackBody)
  trackBody = difference(
    trackBody,
    cylinder({d: wheelHole.diameter, h: 5, fn: wheelHole.segments})
  )

  return translate([8.3, 0, -blockDiameter / 2 - 0.1], trackBody)
}

// generate a full set of track elements
/* - wheeldiameter
   - segmentSize: hole to hole distance for each track segment
   - offset : distance between the centers of the to wheels

   <---------->
    _ _ _ _ _ _
  /   \     /   \
  |   |     |   |
  \_ _/ __  \   /
*/
const computeTrack = (wheelDiameter = 27.65, segmentSize = 12, offset = 54) => {
  // circumeference = 2πr
  // we have length of cord : segmentSize
  // we want to compute NUMBER of cords, given length of cord and diameter of circle

  // to compute number of segments on circle (2 halfs on either side => a full circle)
  const wheelRadius = wheelDiameter / 2
  // we use basic trigonometry for this
  const singleSegmentAngle = 2 * Math.asin((0.5 * segmentSize) / wheelRadius)
  const segmentsOnCircle = 2 * Math.PI / singleSegmentAngle
  const segmentsOnStraightSection = 2 * offset / segmentSize

  const totalSegments = segmentsOnCircle + segmentsOnStraightSection
  console.log('singleSegmentAngle', singleSegmentAngle, 'segmentsOnCircle', segmentsOnCircle)
  console.log('segmentsOnStraightSection', segmentsOnStraightSection)
  console.log('totalSegments', Math.round(totalSegments), totalSegments)

  /* const halfStraigSegments = Math.round(segmentsOnStraightSection / 2)
  const halfCurveSegments = Math.round(segmentsOnCircle / 2)
  const straightSectionTopTrackSegmentTransforms = Array(halfStraigSegments).fill(0)
    .map((_, index) => {
      return {pos: [index * segmentSize, 0, 0], rot: [0, 0, 0]}
    })
  const straightSectionBottomTrackSegmentTransforms = Array(halfStraigSegments).fill(0)
    .map((_, index) => {
      return {pos: [index * segmentSize, 0, wheelRadius], rot: [0, 0, 0]}
    })
  const firstCurveSegments =
  const segmentTransforms = Math.round() */

  return {singleSegmentAngle, segmentsOnStraightSection, segmentsOnCircle, totalSegments}
}

const track = (wheelDiameter = 34.65, segmentSize = 12, offset = 54) => {
  const segments = computeTrack(wheelDiameter, segmentSize, offset)

  return []
  const halfStraigSegments1 = Math.round(segments.segmentsOnStraightSection / 2)
  const halfStraigSegments2 = Math.round(segments.segmentsOnStraightSection) - halfStraigSegments1
  const straightSectionTopTrackSegments = Array(halfStraigSegments1).fill(0)
    .map((_, index) => {
      return translate([index * segmentSize, 0, 0], trackSegment())
    })

  const bottomSectionTopTrackSegments = Array(halfStraigSegments2).fill(0)
    .map((_, index) => {
      return translate([index * segmentSize + segmentSize, 0, -wheelDiameter], rotate([0, 0, 180], trackSegment()))
    })

  //
  const angle = segments.singleSegmentAngle * 180 / Math.PI
  const fooAngle = 180 - 0.5 * angle - 90
  const barAngle = 90 - fooAngle
  const straightxoffset = halfStraigSegments1 * segmentSize - segmentSize
  const halfCurveSegments1 = Math.round(segments.segmentsOnCircle / 2)
  const halfCurveSegments2 = Math.round(segments.segmentsOnCircle) - halfCurveSegments1

  console.log('angle', angle, 'foo', fooAngle, barAngle)

  console.log('halfCurveSegments', halfCurveSegments1, halfCurveSegments2)

  let curTrackSegment = trackSegment()
  const curveSegments1 = Array(halfCurveSegments1).fill(0)
    .map((_, index) => {
      curTrackSegment = translate([segmentSize, 0, 0],
        rotate([0, 46, 0],
          curTrackSegment
      ))
      return curTrackSegment
    })
    .map(cur => translate([straightxoffset, 0, 0], cur))

  curTrackSegment = mirror([1, 0, 0], trackSegment())
  const curveSegments2 = Array(halfCurveSegments2).fill(0)
    .map((_, index) => {
      curTrackSegment = translate([-segmentSize, 0, 0],
        rotate([0, 90 - angle, 0],
          curTrackSegment
      ))
      return curTrackSegment
    })
    .map(cur => translate([0, 0, -wheelDiameter], cur))

  return union(
    straightSectionTopTrackSegments,
    bottomSectionTopTrackSegments,
    curveSegments1,
    curveSegments2
  )
}

module.exports = {trackSegment, track}
