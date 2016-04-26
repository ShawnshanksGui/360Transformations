#pragma once
#include "Layout.hpp"

#include <array>
#include <tuple>
#include <stdexcept>

namespace IMT {
class LayoutEquirectangularTiles : public Layout
{
    public:
        typedef std::tuple<unsigned int, unsigned int> TileId;
        typedef std::array<std::array<std::tuple<unsigned int, unsigned int>, 8>,8> TilesMap;
        virtual ~LayoutEquirectangularTiles() = default;

        virtual NormalizedFaceInfo From2dToNormalizedFaceInfo(const CoordI& pixel) const override;
        virtual CoordF FromNormalizedInfoTo2d(const NormalizedFaceInfo& ni) const override;
        virtual NormalizedFaceInfo From3dToNormalizedFaceInfo(const Coord3dSpherical& sphericalCoord) const override;
        virtual Coord3dCart FromNormalizedInfoTo3d(const NormalizedFaceInfo& ni) const override;

    private:
        struct TileResolutions
        {
            public:
                TileResolutions(void) = delete;
                TileResolutions(TilesMap tileResVect):
                    m_tiles(std::move(tileResVect))
                    {}
                const std::tuple<unsigned int, unsigned int>& GetRes(const TileId& t) const
                {
                    auto i = std::get<0>(t);
                    auto j = std::get<1>(t);
                    if (i >= 8 || j >= 8)
                    {
                        throw std::invalid_argument("GetRes: invalide tile id -> ("+std::to_string(i)+","+std::to_string(j)+")");
                    }
                    return m_tiles[i][j];
                }
                unsigned int GetResWidth(const TileId& t) const
                {

                    return std::get<0>(GetRes(t));
                }
                unsigned int GetResHeight(const TileId& t) const
                {

                    return std::get<1>(GetRes(t));
                }

            private:
                TilesMap m_tiles;
        };


        void InitImpl(void) override
        {
            unsigned int sumWidth(0), sumHeight(0);
            for(unsigned int i = 0; i < 8; ++i)
            {
                unsigned int maxRow = 0;
                unsigned int maxCol = 0;
                for (unsigned int j = 0; j < 8; ++j)
                {
                    maxRow = MAX(maxRow, m_tr.GetResHeight(std::make_tuple(j,i)));
                    maxCol = MAX(maxCol, m_tr.GetResWidth(std::make_tuple(i,j)));
                }
                sumHeight += maxRow;
                m_rowsMaxSize[i] = maxRow;
                sumWidth += maxCol;
                m_colsMaxSize[i] = maxCol;
            }
            SetWidth(sumWidth);
            SetHeight(sumHeight);
            for (unsigned i = 0; i < 8; ++i)
            {
                for (unsigned j = 0; j < 8; ++j)
                {
                    unsigned int offI(0), offJ(0);
                    for (unsigned k = 0; k < i; ++k)
                    {
                        offI += m_colsMaxSize[k];
                    }
                    for (unsigned k = 0; k < j; ++k)
                    {
                        offJ += m_rowsMaxSize[k];
                    }
                    if (i != 0)
                    {
                        offI += (m_colsMaxSize[i]-m_tr.GetResWidth(std::make_tuple(i,j)))/2;
                    }
                    if (j != 0)
                    {
                        offJ += (m_rowsMaxSize[j]-m_tr.GetResHeight(std::make_tuple(i,j)))/2;
                    }
                    m_offsets[i][j] = CoordI(offI, offJ);
                }
            }
        }

        TileId From2dToTileId(unsigned int i, unsigned int j) const;
        const CoordI& TileIdTo2dOffset(const TileId& ti) const {return TileIdTo2dOffset(std::get<0>(ti),std::get<1>(ti));}
        const CoordI& TileIdTo2dOffset(const unsigned int& i, const unsigned int& j) const;
        CoordI TileIdTo2dEndOffset(const TileId& ti) const;
        TileResolutions m_tr;
        std::array<unsigned int, 8> m_rowsMaxSize;
        std::array<unsigned int, 8> m_colsMaxSize;
        std::array<std::array<CoordI,8>,8> m_offsets;

    public:

        LayoutEquirectangularTiles(TilesMap tr): m_tr(std::move(tr)),
        m_rowsMaxSize(), m_colsMaxSize(), m_offsets() {};

};
}
