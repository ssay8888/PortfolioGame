#pragma once
#include <Windows.h>
#include <list>
#include <functional>
#include <math.h>
#include <iostream>

namespace CollisionManager
{

	template<typename T>
	bool Check_Sphere(const T* pDstObject, const T& pSrcObject)
	{
		// �� ���浹�� �غ����� 
		// ���� 1. �� ���� �������� ���� ���ض�! 
		float fRadiusSum = (pDstObject->Get_Info()->iCX >> 1) + (pSrcObject.Get_Info()->iCX >> 1);
		// ���� 2. �غ��� ���̸� ���϶�!
		float fX = pDstObject->Get_Info()->fX - pSrcObject.Get_Info()->fX; // �غ� ���ϱ� �Ϸ�! 
		float fY = pDstObject->Get_Info()->fY - pSrcObject.Get_Info()->fY; // ���� ���ϱ� �Ϸ�! 

		//���� 3. �� ���� �Ÿ��� ���϶�! �װ͵� ��Ÿ����� ������ �̿��Ͽ�! C^2 = a^2 + b^2
		float fDist = sqrtf(fX * fX + fY * fY);
		//���� 4. ���ض�! �Ÿ��� �������� ����! ���� �Ÿ��� �� �۴ٶ�°�! �浹�ߴ� ��� ���̴�!!!! 

		return fRadiusSum >= fDist;
	}

	template<typename T>
	bool Check_Rect(const T* pDstObject, const T* pSrcObject, float* pX, float* pY)
	{
		// 1. x�������� �Ÿ��� x �������� �������� ��. 
		float fDistX = std::fabs(pDstObject->GetInfo().x - pSrcObject->GetInfo().x);
		float fRadiusSumX = (pDstObject->GetInfo().cx * 0.5f) + (pSrcObject->GetInfo().cx >> 1);

		// 2. y�������� �Ÿ��� y�������� �������� ���� ���Ҳ����� >_<
		float fDistY = std::fabs(pDstObject->GetInfo().y - pSrcObject->GetInfo().y);
		float fRadiusSumY = static_cast<float>((pDstObject->GetInfo().cy / 2) + (pSrcObject->GetInfo().cy >> 1));

		// ���� ���Ҳ�����!? 
		//�񱳿�!!!! ���� ���� !? 1���� 2���� �� !! �� ��! 
		if (fDistX <= fRadiusSumX && fDistY <= fRadiusSumY)
		{
			*pX = fRadiusSumX - fDistX;
			*pY = fRadiusSumY - fDistY;
			return true;
		}
		return false;
	}

	template<typename T>
	void Collision_RectList(std::list<T*>& rlistDst, std::list<T*>& rlistSrc, std::function<void> f)
	{
		RECT rc{};
		for (auto& pDst : rlistDst)
		{
			for (auto& pSrc : rlistSrc)
			{
				if (IntersectRect(&rc, &pDst->Get_Rect(), &pSrc->Get_Rect()))
				{
					f(pDst);
				}
			}
		}
	}
	template<typename T>
	void Collision_RectEX(std::list<T*>& rlistDst, std::list<T*>& rlistSrc)
	{
		float fX = 0.f, fY = 0.f;
		for (auto& pDst : rlistDst)
		{
			for (auto& pSrc : rlistSrc)
			{
				if (Check_Rect(pDst, pSrc, &fX, &fY))
				{
					float fSrcX = pSrc->GetInfo().x;
					float fSrcY = pSrc->Get_Info().y;

					if (fX > fY)
					{
						if (fSrcY <= pDst->GetInfo().y)
							fY *= -1.f;

						pSrc->SetPos(fSrcX, fSrcY + fY);
					}
					else
					{
						if (fSrcX <= pDst->GetInfo().x)
							fX *= -1.f;

						pSrc->SetPos(fSrcX + fX, fSrcY);
					}
				}
			}
		}
	}

	template<typename T>
	void Collision_RectEX(T* dst, T* src)
	{
		float fX = 0.f, fY = 0.f;
		if (CollisionManager::Check_Rect<T>(dst, src, &fX, &fY))
		{
			float fSrcX = src->GetInfo().x;
			float fSrcY = src->GetInfo().y;

			if (fX > fY)
			{
				if (fSrcY <= dst->GetInfo().y)
					fY *= -1.f;
				if (fY < 10 && fY > -10)
				src->SetPos(fSrcX, fSrcY + fY);

				if (fSrcY > 0 || fY > 0)
					std::cout << fSrcY << " : " << fY << std::endl;
			}
			else
			{
				if (fSrcX <= dst->GetInfo().x)
					fX *= -1.f;

				if (fX < 10 && fX > -10)
				src->SetPos(fSrcX + fX, fSrcY);
			}
		}
	}

	template<typename T>
	void Collision_Sphere(std::list<T*>& rlistDst, std::list<T*>& rlistSrc)
	{
		for (auto& pDst : rlistDst)
		{
			for (auto& pSrc : rlistSrc)
			{
				if (Check_Sphere(pDst, *pSrc))
				{

				}
			}
		}
	}




};

