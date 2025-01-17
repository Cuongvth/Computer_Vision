﻿using CMS_WebDesignCore.DTO;
using CMS_WebDesignCore.Entities.Entities_OCR;
using CMS_WebDesignCore.Enums;
using CMS_WebDesignCore.Wrap;
using Microsoft.AspNetCore.Http;

namespace CMS_WebDesignCore.IBusiness.IBusiness_OCR
{
    public interface IAdminService
    {

        Task<IQueryable<CanCuocCongDan>> GetCanCuocPage(int page, int pageSize);
        Task<int> GetCanCuocCount();
        Task<int> GetBLXCount();
        Task<int> GetDuLieuCount();
        Task<IQueryable<GiayPhepLaiXe>> GetBLX(int id);
        Task<IQueryable<CanCuocCongDan>> GetCanCuoc(int id);
        Task<IQueryable<DuLieu>> GetDuLieuPage(int page, int pageSize);
        Task<IQueryable<GiayPhepLaiXe>> GetBLXPage(int page, int pageSize);
        Task<ActionStatus> ThemDuLieu(IFormFile matTruoc, IFormFile matSau);
        Task<GoogleIdenDTO<CanCuocCongDan>> NhanDangCCCD(int duLieuId);
        Task<GoogleIdenDTO<GiayPhepLaiXe>> NhanDangBLX(int duLieuId);
        Task<CheckResult> NhanDang(int duLieuId);
        Task<ActionStatus> ThongTinCoChinhXacCCCD(int CCCDID, int isSo, int isHoTen, int isCoGiaTriDen, int isNgayThang, int isGioiTinh, int isQuocTich, int isQueQuan, int isvnm, int isNoiThuongTru, int isDacDien, int isNgayDangKi, int isMatTruoc, int isMatSau, bool isLoaiThe);
        Task<ActionStatus> ThongTinCoChinhXacBLX(int BLXID, int isSo, int isHoTen, int isNoiCuTru, int isNgayThang, int isQuocTich, int isHang, int isMoTaXeDuocSuDung, int isNgayTrungTuyen, int isNgayDangKi, int isMatTruoc, int isMatSau, bool isLoaiThe);
        Task<GoogleIdenDTO<CanCuocCongDan>> NhanDangCCCDTrucTiep(IFormFile matTruoc, IFormFile matSau);
        Task<GoogleIdenDTO<GiayPhepLaiXe>> NhanDangBLXTrucTiep(IFormFile matTruoc, IFormFile matSau);
        Task<CheckResult> NhanDangTrucTiep(IFormFile matTruoc);
        Task fake();
    }
}
