

class Section
{
public:
   Section( unsigned int u32Id, unsigned int u32Lenth, bool bIsJunction);

private:
    Section(){}

	//! Unique ID of the section/Track
	unsigned int m_u32Id;
    //! Length of the section/Track
	unsigned int m_u32LenthOfTheSection;
	//! Is this section a junction
    bool m_bIsThisSectionJunction;
};
